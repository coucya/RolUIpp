
#include <climits>

#include "RolUI/Window.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {

    Window::Window() noexcept {
        _init_event_bind();
    }
    Window::~Window() {}

    void Window::_init_event_bind() noexcept {
        on_size_change.connect(this, &Window::_do_size_change);
    }
    void Window::_do_size_change(Size s) noexcept {
        if (_content_widget)
            _content_widget->_update_size_and_pos();
    }

    IPainter* Window::painter() { return nullptr; }
    void Window::begin_draw() {}
    void Window::end_draw() {}

    void Window::dispatch_event(double timeout) {}

    Application* Window::application() const noexcept { return _application; }

    void Window::set_content_widget(Widget* widget) noexcept {
        if (widget == nullptr) return;

        _content_widget = widget;
        _content_widget->_do_window_change(_content_widget->_window, this);
        _content_widget->_update_size_and_pos();
    }

    void Window::_draw_widget(RolUI::Widget* widget, Point base_pos, Rect scissor, RolUI::IPainter* painter) noexcept {
        if (widget == nullptr || painter == nullptr) return;

        Point pos = widget->pos();
        Size size = widget->size();
        Point new_base_pos = base_pos + pos;
        auto new_scissor_opt = scissor.intersected({new_base_pos, size});
        Rect new_scissor = new_scissor_opt.has_value() ? new_scissor_opt.value() : Rect{scissor.pos(), {0, 0}};

        widget->on_draw(painter);

        painter->set_base_pos(new_base_pos);
        painter->scissor(new_scissor);

        for (auto& cw : widget->_children) {
            _draw_widget(cw, new_base_pos, new_scissor, painter);
        }

        painter->scissor(scissor);
        painter->set_base_pos(base_pos);
    }
    void Window::draw() noexcept {
        Widget* root_widget = this->_content_widget;
        IPainter* painter = this->painter();

        if (root_widget == nullptr) return;
        if (painter == nullptr) return;

        begin_draw();
        _draw_widget(root_widget, Point(0, 0), Rect{Point(), this->size()}, painter);
        end_draw();
    }

    Widget* Window::get_widget_by_pos(Point pos) const noexcept {
        Point t_pos = pos;

        Widget* widget = nullptr;
        Widget* it_w = _content_widget;
        while (it_w && it_w->rect().contain(t_pos)) {
            widget = it_w;
            t_pos = t_pos - it_w->pos();
            it_w = it_w->get_child_by_pos(t_pos);
        }
        return widget;
    }

} // namespace RolUI
