
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
        _init_event_callback();
    }

    GLFWWindow::~GLFWWindow() {}

    void GLFWWindow::_init_nanovg() {
        _nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        _painter = GLFWPainter(_nvg_context);
    }
    void GLFWWindow::_init_event_callback() {
        on_cursor_enter = [this](int enter) {
            auto [mx, my] = this->cursor_pos();
            this->_mouse_dispatcher.set_pos((int32_t)mx, (int32_t)my);
            this->_mouse_dispatcher.set_last_pos((int32_t)mx, (int32_t)my);
        };
        on_cursor_pos = [this](double x, double y) {
            this->_mouse_dispatcher.set_pos((int)x, (int)y);
        };
        on_mouse_button = [this](int button, int action, int mods) {
            RolUI::MouseKey key;
            RolUI::MouseKeyMode md;
            switch (button) {
            case GLFW_MOUSE_BUTTON_1: key = RolUI::MouseKey::key1; break;
            case GLFW_MOUSE_BUTTON_2: key = RolUI::MouseKey::key2; break;
            case GLFW_MOUSE_BUTTON_3: key = RolUI::MouseKey::key3; break;
            case GLFW_MOUSE_BUTTON_4: key = RolUI::MouseKey::key4; break;
            case GLFW_MOUSE_BUTTON_5: key = RolUI::MouseKey::key5; break;
            case GLFW_MOUSE_BUTTON_6: key = RolUI::MouseKey::key6; break;
            case GLFW_MOUSE_BUTTON_7: key = RolUI::MouseKey::key7; break;
            case GLFW_MOUSE_BUTTON_8: key = RolUI::MouseKey::key8; break;
            };
            switch (action) {
            case GLFW_PRESS: md = RolUI::MouseKeyMode::press; break;
            case GLFW_RELEASE: md = RolUI::MouseKeyMode::release; break;
            };
            this->_mouse_dispatcher.set_key_mode(key, md);
        };

        on_size = [this](int w, int h) {
            glViewport(0, 0, w, h);
        };

        _mouse_dispatcher.clear_change();
    }

    RolUI::IPainter* GLFWWindow::painter() { return &_painter; }

    void GLFWWindow::begin_draw() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        if (vg == nullptr) return;

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, width(), height(), 1);
    }
    void GLFWWindow::end_draw() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        if (vg == nullptr) return;
        nvgEndFrame(vg);
        swap_buffer();
    }

    void GLFWWindow::dispatch_event() {
        this->wait_events();

        _mouse_dispatcher.dispatch(this);
        _mouse_dispatcher.clear_change();
    }

} // namespace RolUIBackend
