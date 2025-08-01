GLFW_DIR = $(MODEL_PACKAGE_HOME)/glfw
BUILD_DIR := $(GLFW_DIR)/build

.PHONY: all clean configure

all: build

configure:
	@echo "Configuring GLFW..."
	@mkdir -p $(BUILD_DIR)
	@cmake -S $(GLFW_DIR) -B $(BUILD_DIR)

build: configure
	@echo "Building glfw..."
	@cmake --build $(BUILD_DIR)

clean: 
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)