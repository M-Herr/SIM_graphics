MODEL_PACKAGE_HOME = $(PWD)

include $(MODEL_PACKAGE_HOME)/makefiles/glfw.mk

TRICK_CFLAGS += -g -std=c++17 -I./models -I./glfw/include/ -I./models/imgui -I./models/imgui/backends
TRICK_CXXFLAGS += -g  -std=c++17 -I./models -I./glfw/include/ -I./models/imgui -I./models/imgui/backends

TRICK_LDFLAGS += -L/glfw/build/src 
TRICK_USER_LINK_LIBS += glfw/build/src/libglfw3.a -lGL

TRICK_ICG_EXCLUDE = $(MODEL_PACKAGE_HOME)/models/imgui:$(MODEL_PACKAGE_HOME)/models/threads/include/glad.h