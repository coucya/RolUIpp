#include <stdexcept>

#include "glad/glad.h"

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

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
        : _glfw_window(nullptr), _painter(nullptr) {

        if (!_is_init) _init();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _glfw_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        if (_glfw_window == nullptr)
            throw std::runtime_error("glfw create window error.");

        make_opengl_context();
        init_nanovg();
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

    void GLFWWindow::init_nanovg() {
        _nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        if (_nvg_context == nullptr)
            throw std::runtime_error("RolUIBackend::GLFWWindow::init_nanovg(): nvg_context == nullptr.");
        _painter = GLFWPainter(_nvg_context);
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
            draw();

            poll_events();
            display();
        }
    }
    void GLFWWindow::draw() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, width(), height(), 1);

        RolUI::Window::draw();

        nvgEndFrame(vg);
    }
    RolUI::IPainter* GLFWWindow::painter() {

        return &_painter;
    }

} // namespace RolUIBackend
