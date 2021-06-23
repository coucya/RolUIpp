#include <stdexcept>

#include "glad/glad.h"

#include "GLFWWindow.h"
#include "GLFWPainter.h"

namespace RolUIBackend {
    bool GLFWWindow::_is_init = false;
    void GLFWWindow::_init() {
        if (_is_init) return;

        if (glfwInit() == GLFW_FALSE)
            throw std::runtime_error("glfw initialization error.");

        _is_init = true;
    }

    GLFWWindow::GLFWWindow(size_t w, size_t h, const char* title)
        : _glfw_window(nullptr), RolUI::Window() {

        if (!_is_init) _init();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _glfw_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        if (_glfw_window == nullptr)
            throw std::runtime_error("glfw create window error.");
    }

    GLFWWindow::~GLFWWindow() {
        if (_glfw_window != nullptr)
            glfwDestroyWindow(_glfw_window);
    }

    void GLFWWindow::make_opengl_context() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwMakeContextCurrent(_glfw_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("glfw make OpenGL context error");
    }

    size_t GLFWWindow::width() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        int w, h;
        glfwGetWindowSize(_glfw_window, &w, &h);
        return w;
    }
    size_t GLFWWindow::height() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        int w, h;
        glfwGetWindowSize(_glfw_window, &w, &h);
        return h;
    }

    bool GLFWWindow::should_close() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        return glfwWindowShouldClose(_glfw_window) == GLFW_TRUE;
    }

    void GLFWWindow::poll_events() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwPollEvents();
    }
    void GLFWWindow::wait_events() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwWaitEvents();
    }

    void GLFWWindow::hide() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwHideWindow(_glfw_window);
    }
    void GLFWWindow::show() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwShowWindow(_glfw_window);
    }

    void GLFWWindow::display() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwSwapBuffers(_glfw_window);
    }

    void GLFWWindow::close() {
        if (!_is_init) throw std::runtime_error("glfw is not initialized.");

        glfwSetWindowShouldClose(_glfw_window, GLFW_TRUE);
    }

    void GLFWWindow::run() {
        while (should_close() == false) {
            poll_events();
            display();
        }
    }

    RolUI::IPainter* GLFWWindow::painter() {

        return nullptr;
    }

} // namespace RolUIBackend
