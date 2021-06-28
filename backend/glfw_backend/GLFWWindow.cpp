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
            this->_mouse_state.last_pos = {(int32_t)mx, (int32_t)my};
            this->_mouse_state.current_pos = {(int32_t)mx, (int32_t)my};
        });
        set_cursor_pos_callback([this](double x, double y) {
            this->_mouse_state.last_pos = this->_mouse_state.current_pos;
            this->_mouse_state.current_pos = {(int32_t)x, (int32_t)y};

            this->_distribute_mouse_event();
        });

        while (!should_close()) {
            draw();

            poll_events();
            swap_buffer();
        }
    }

    bool GLFWWindow::_distribute_mouse_event_to_widget(
        RolUI::Widget* w, RolUI::Point widget_pos) {

        auto chilren = w->children_view_reverse();

        bool is_do = false;
        for (auto& c : chilren) {
            RolUI::Point c_widget_pos = widget_pos + c.pos();
            is_do = _distribute_mouse_event_to_widget(&c, c_widget_pos);
            if (is_do) break;
        }

        RolUI::Point mouse_pos = _mouse_state.current_pos - widget_pos;
        RolUI::Rect widget_rect = RolUI::Rect{RolUI::Point(), w->size()};

        if (!is_do && widget_rect.is_contain_point(mouse_pos)) {
            RolUI::MouseEvent event{_mouse_state.last_pos, _mouse_state.current_pos};
            event.set_widget_pos(widget_pos);
            is_do = w->do_event(&event);
        }

        return is_do;
    }
    void GLFWWindow::_distribute_mouse_event() {
        RolUI::Widget* root_widget = widget();
        if (root_widget == nullptr) return;

        _distribute_mouse_event_to_widget(root_widget, root_widget->pos());
    }

} // namespace RolUIBackend
