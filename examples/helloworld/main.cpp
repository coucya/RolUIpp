#include <iostream>

#include "glfw_backend/GLFWWindow.h"

int main(int argc, char* argv[]) {
    RolUIBackend::GLFWWindow win(800, 600, "helloworld");
    win.show();

    win.run();

    return 0;
}