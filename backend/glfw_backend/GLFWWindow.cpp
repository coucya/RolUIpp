#include "glad/glad.h"

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "RolUI/events/MouseEvent.h"

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

        set_cursor_enter_callback([this](int enter) {
            auto [mx, my] = this->cursor_pos();

            this->mouse_dispatcher().set_pos((int32_t)mx, (int32_t)my);
            this->mouse_dispatcher().set_last_pos((int32_t)mx, (int32_t)my);
        });
        set_cursor_pos_callback([this](double x, double y) {
            this->mouse_dispatcher().set_pos((int32_t)x, (int32_t)y);
        });

        while (!should_close()) {
            poll_events();

            distribute_event();

            draw();

            swap_buffer();
        }
    }

} // namespace RolUIBackend
