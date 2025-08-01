/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    (
    )
@endverbatim
**/
#pragma once
#include <functional>
#include <string>
#include <vector>
#include <atomic>
#include <variant>
#include <string_view>
#include <optional>
#include <ostream>
#include <type_traits>
#include <sstream>

#include "imgui.h"
#include "UIScopes.hh"
#include "trick/parameter_types.h"


namespace UI
{
    template<class... Ts> 
    struct overloaded : Ts... 
    {
        using Ts::operator()...; 
    };

    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    static inline bool plausible_user_ptr(const void* vp) {
        auto u = reinterpret_cast<uintptr_t>(vp);
        // crude userland x86-64 range check; adjust if needed
        return u > 0x10000ull && u < 0x0000800000000000ull;
        }


    struct Text 
    { 

        struct AnyRef {
            const void* ptr{nullptr};
            void (*format)(const void*, std::string&);
        };

        using StringSource = std::variant<
            std::monostate,
            const char*,
            std::reference_wrapper<const std::string>,
            std::string_view,
            AnyRef
            >;
        

        template<class T, class = void>
        struct is_streamable : std::false_type {};
        template<class T>
        struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>
            : std::true_type {};

        template<class T>
        static constexpr bool is_streamable_v = is_streamable<T>::value;

        template<class T>
        struct dependent_false : std::__false_type {};

       

        template <class T>
        static  AnyRef make_anyref(const T& t) 
        {   
            using U = std::decay_t<T>;

             // --- Pointer case: format *ptr (live value), not the pointer address
            if constexpr (std::is_pointer<U>::value) {
                using P = std::remove_pointer_t<U>;
                // char*/const char* should use the c-string path instead
                static_assert(!std::is_same<std::remove_cv_t<P>, char>::value,
                            "Use Text(const char*)/set_cstr for char*");

                const P* p = t; // store the pointer's value
                return  AnyRef{
                    p,
                    [](const void* vp, std::string& out)
                    {
                        if(!vp || !plausible_user_ptr(vp)) { out = "<bad-ptr>"; return;}
                        
                        if(!vp) { out = "<null>"; return;}
                        const P& v = *static_cast<const P*>(vp);
                        if constexpr (std::is_same<P, bool>::value)
                        {
                            bool v = *static_cast<const bool*>(vp);
                            out = v ? "true" : "false";
                            //oss << std::boolalpha << v;
                            return;
                        } 
                        else if constexpr(std::is_arithmetic<P>::value)
                        {
                            out = std::to_string(v);
                        }
                        else if constexpr (is_streamable_v<P>) 
                        {
                            std::ostringstream oss;
                            oss << v;
                            out = std::move(oss).str();
                        }
                        else
                        {
                            out = "<unprintable>";
                        }
                    }
                };
            }

            using U = std::decay_t<T>;
            if constexpr (std::is_arithmetic<U>::value) 
            {
                return AnyRef 
                {
                    &t,
                    [](const void* p, std::string& out)
                    {
                        const U& v = *static_cast<const U*>(p);
                        out = std::to_string(v);
                    }
                };
            } 
            else if constexpr (is_streamable_v<U>) 
            {
                return AnyRef
                {
                    &t,
                    [](const void* p, std::string& out)
                    {
                        const U& v = *static_cast<const U*>(p);
                        std::ostringstream oss;
                        oss << v;
                        out = std::move(oss).str();
                    }

                };
            } 
            else 
            {
                static_assert(dependent_false<U>::value,
                            "Text: type is neither arithmetic nor streamable with operator<<");
            }
        }

        Text() = default;
        explicit Text(std::string in_owned) 
            : owned(std::move(in_owned)), source{std::monostate{}} {}               // owned

        explicit Text(const char* cstr) 
            : source(cstr) {}                                                       // c-string'

        template<size_t N>
        explicit Text(const char (&arr)[N])
            : source(arr) {}

        explicit Text(std::string_view sv)
            : source(sv) {}   
                                
        template<class T,
            std::enable_if_t<
                !std::is_same<std::decay_t<T>, std::string>::value &&
                !std::is_same<std::decay_t<T>, const char*>::value &&
                !std::is_same<std::decay_t<T>, char*>::value &&
                !std::is_same<std::decay_t<T>, std::string_view>::value,
                int> = 0
            >
        explicit Text(const T& ref) 
        {
            set_ref(ref);
        }

        void set_owned(std::string s)              
        {
            owned = std::move(s); 
            source = std::monostate{}; 
        }
        
        void set_cstr(const char* s)               
        { 
            source = s; 
        }
        
        template <std::size_t N> 
        void set_cstr(const char (&arr)[N]) 
        { 
            source = arr; 
        }
        
        void set_view(std::string_view sv)         
        { 
            source = sv; 
        }
        
        void set_ref(const std::string& s)         
        { 
            source = std::cref(s); 
        }

        template<class T>
        void set_ref(const T& t)
        {
            source = make_anyref(t);
        }

        template<class T, class F>
        void set_ref(const T& t, F&& formatter)
        {
            source = AnyRef{
                &t,
                [fn = std::forward<F>(formatter)](const void* p, std::string& out)
                {
                    fn(*static_cast<const T*>(p), out);
                }
            };
        }

        void draw()
        {
            std::string_view sv = get_view();
            ImGui::TextUnformatted(sv.data(), sv.data() + sv.size());
        }


        std::string_view get_view()
        {
            return std::visit(overloaded 
            {
                [&](std::monostate) -> std::string_view 
                {
                    return std::string_view{owned};
                },
                [&](const char* s) -> std::string_view
                {
                    if(!s) return std::string_view{};
                    return std::string_view{s};
                },
                [&](std::reference_wrapper<const std::string> ref) -> std::string_view 
                {
                    return std::string_view{ref.get()};
                },
                [&](std::string_view sv) -> std::string_view
                {
                    return sv;
                },
                [&](AnyRef ar) 
                {
                    cache.clear();
                    ar.format(ar.ptr, cache);
                    return std::string_view(cache);
                }
            }, source);
        }

        private:
        std::string owned;
        StringSource source;
        std::string cache;
    };

    inline Text make_text_from_trick_ptr(void* p, TRICK_TYPE t)
    {
        switch(t) 
        {
        case TRICK_CHARACTER:           return Text(static_cast<char*>(p));
        case TRICK_STRING: {
            const char* s = p ? *static_cast<char* const*>(p) : nullptr;
            return Text(s);
        }             
        case TRICK_SHORT:               return Text(static_cast<short*>(p));
        case TRICK_INTEGER:             return Text(static_cast<int*>(p));
        case TRICK_LONG:                return Text(static_cast<long*>(p));
        case TRICK_LONG_LONG:           return Text(static_cast<long long*>(p));
        case TRICK_UNSIGNED_SHORT:      return Text(static_cast<unsigned short*>(p));
        case TRICK_UNSIGNED_INTEGER:    return Text(static_cast<unsigned int*>(p));
        case TRICK_UNSIGNED_LONG:       return Text(static_cast<unsigned long*>(p));
        case TRICK_UNSIGNED_LONG_LONG:  return Text(static_cast<unsigned long long*>(p));
        case TRICK_DOUBLE:              return Text(static_cast<double*>(p));
        case TRICK_FLOAT:               return Text(static_cast<float*>(p));
        case TRICK_BOOLEAN:             return Text(static_cast<bool*>(p));
        default:
            return Text("<UNSUPPORTED TYPE>");
        
        }
        
    }

    struct Button 
    { 
        std::string label; 
        std::function<void()> on_click; 
    };

    struct Checkbox 
    { 
        std::string label; 
        bool* ptr; 
    };

    struct Child 
    { 
        std::string id; 
        ImVec2 size{0,0}; 
        bool border=false; 
        ImGuiWindowFlags flags=0; 
        std::vector<struct Node> children;
    };

    struct Window 
    { 
        std::string title; 
        bool open=true; 
        ImGuiWindowFlags flags=0; 
        std::vector<struct Node> children;
    };

    struct LabelValue 
    {
        std::string id;
        Text label;
        Text value;
        ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchProp;
    };

    struct TreeNode
    {
        std::string id;
        Text label;
        std::optional<Text> value;
        std::vector<struct Node> children;

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanFullWidth;

        bool default_open = false;
        std::function<void()> on_click;
        std::function<void()> on_context_menu;

    };

    struct Node
    {
        using Variant = std::variant<Text, Button, Checkbox, Child, Window, LabelValue, TreeNode>;
        Variant nodeVariant;

        template<class T, class = std::enable_if<!std::is_same<std::decay_t<T>, Node>::value>>
        Node(T&& x) : nodeVariant(std::forward<T>(x)) {}
        Node() = default;
    };
    
    inline void draw(Node&);

    inline void draw_children(std::vector<Node>& v) 
    {
        for (auto& n : v) 
        {
            draw(n); 
        }
    }

    inline void draw_tree_node(TreeNode& tn)
    {
        ImGui::PushID(tn.id.c_str());
        if(tn.default_open)
        {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        }

        ImGuiTreeNodeFlags flags = tn.flags;
        const bool has_children = !tn.children.empty();
        if(!has_children)
        {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        const auto lbl = tn.label.get_view();
        std::string display(lbl.data(), lbl.size());
        
        if(tn.value) 
        {
            const auto val = tn.value->get_view();
            if(!val.empty())
            {
                display += " ";
                display.append(val.data(), val.size());
            }
        }

        const bool open = ImGui::TreeNodeEx("##node", flags, "%s", display.c_str());

        if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            if(tn.on_click) tn.on_click();
        }

        if(ImGui::BeginPopupContextItem())
        {
            if(tn.on_context_menu) tn.on_context_menu();
            ImGui::EndPopup();
        }

        if(has_children && open)
        {
            for(auto& child : tn.children)
            {
                draw(child);
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    inline void draw(Node& n)
    {
        std::visit(overloaded 
        {
            [](Text& t) 
            {
                t.draw();
            },
            [](Button& b) 
            {
                if(ImGui::Button(b.label.c_str())) 
                {
                    if (b.on_click) b.on_click(); 
                }
            },
            [](Checkbox& c) 
            {
                bool chkbox_temp = c.ptr ? *c.ptr : false;
                if(ImGui::Checkbox(c.label.c_str(), &chkbox_temp)) { if (c.ptr) *c.ptr = chkbox_temp;}
            },
            [](Child& ch)
            {
                ChildScope s(ch.id.c_str(), ch.size, ch.border, ch.flags);
                draw_children(ch.children);
            },
            [](Window& w)
            {
                if(!w.open) return;
                WindowScope s(w.title.c_str(), &w.open, w.flags);
                if(s) draw_children(w.children);
            },
            [](LabelValue& lv)
            {
                TableScope ts(lv.id.c_str(), 2, lv.flags);
                if(!ts) return;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                lv.label.draw();
                ImGui::TableSetColumnIndex(1);
                lv.value.draw();
            },
            [](TreeNode& tn)
            {
               draw_tree_node(tn);
            }
        }, n.nodeVariant);
    }    
}