#include "UI/include/MainWindow.hh"

#include "trick/message_proto.h"

namespace UI {

    static void glfw_error_callback(int error, const char* description)
    {
        message_publish(1, "GLFW Error: %d, %s\n", error, description);
    }

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    MainWindow::MainWindow(const WindowProperties& in_properties)
    :running(true), properties(in_properties)
    {

    }

    MainWindow::~MainWindow()
    {   

    }

    bool MainWindow::InitializeGlad()
    {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            message_publish(1, "GLAD Error: Could not initialize.\n");
            return false;
        }

        //const GLubyte* glsl_version = "#version 460";
        //glslVersion = glsl_version ? reinterpret_cast<const char*>(glsl_version) : "unkown glsl version";
        glslVersion = "#version 460";

        const GLubyte* renderer = glGetString(GL_RENDERER);
        const char* renderer_string = renderer ? reinterpret_cast<const char*>(renderer) : "unkown renderer";

        message_publish(1, "Initialized GLAD; OpenGL Renderer: %s, OpenGL from glad %d.%d\n", renderer_string, GLVersion.major, GLVersion.minor);
        return true;
    }

    bool MainWindow::InitializeGLFW()
    {   
        glfwSetErrorCallback(glfw_error_callback);

        if(!glfwInit())
        {   
            //ConsoleLogger::getInstance()->error("GLFW failed to initialize!");
            message_publish(1, "GLFW failed to initialize.\n");
            return false;
        }

        glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
        glfwWindowHint(GLFW_DEPTH_BITS,24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

             //Figure out GL + GLSL version
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        //const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        //const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 4.3 + GLSL 150
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        message_publish(1, "Creating window: \"%s\" (%zu x %zu)\n",
                    properties.name.c_str(), properties.width, properties.height);

        //glslVersion = std::string(glsl_version);

        window = glfwCreateWindow(properties.width, properties.height, properties.name.c_str(), nullptr, nullptr);
        if(!window)
        {
            message_publish(1, "GLFW failed to create a window.\n");   
            return false;
        }

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        
        message_publish(1, "OpenGL from GLFW Version: %d.%d\n", glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR), glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR));
        return true;
        
    }

    bool MainWindow::SetupImgui()
    {   

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;     // Enable Keyboard Controls
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        if(!ImGui_ImplGlfw_InitForOpenGL(window, true)) 
        {
            return false;
        }

        if(!ImGui_ImplOpenGL3_Init(glslVersion.c_str()))
        {
            return false;
        }


        return true;
    }

    void MainWindow::BeginFrame()
    {
        glfwPollEvents();

        //Imgui Frame stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void MainWindow::EndFrame()
    {
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(properties.clear_color.x * properties.clear_color.w, 
            properties.clear_color.y * properties.clear_color.w, 
            properties.clear_color.z * properties.clear_color.w, 
            properties.clear_color.w);

        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        if(glfwWindowShouldClose(window)) 
        {
            running = false;
        }
    }


    void MainWindow::CleanUp()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
       // glfwTerminate();
    }

    
}