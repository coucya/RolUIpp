#include "glad/glad.h"

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "GLFWWindow.h"

namespace RolUIBackend {

    GLFWWindow::GLFWWindow(uint32_t w, uint32_t h, const char* title)
        : GLFWppWindow(w, h, title), _painter(nullptr) {
        make_opengl_context();
        _init_nanovg();
    }

    GLFWWindow::~GLFWWindow() {}

    void GLFWWindow::_init_nanovg() {
        _nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        if (_nvg_context == nullptr)
            throw std::runtime_error("RolUIBackend::GLFWWindow::init_nanovg(): nvg_context == nullptr.");
        _painter = GLFWPainter(_nvg_context);
    }

    RolUI::IPainter* GLFWWindow::painter() {
        return &_painter;
    }

    void GLFWWindow::draw() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, width(), height(), 1);

        RolUI::Window::draw();

        nvgEndFrame(vg);
    }

    void GLFWWindow::run() {

        while (!should_close()) {
            draw();

            poll_events();
            swap_buffer();
        }
    }
} // namespace RolUIBackend