#include <iostream>

#include "glfw_backend/GLFWWindow.h"

int main(int argc, char* argv[]) {
    RolUIBackend::GLFWWindow win(800, 600, "helloworld");
    win.show();

    while (win.should_close() == false) {
        win.poll_events();
        win.display();
    }

    win.close();

    return 0;
}