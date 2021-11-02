
#include "RolUI/Window.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {

    IPainter* Window::painter() { return nullptr; }
    void Window::begin_draw() {}
    void Window::end_draw() {}

    void Window::dispatch_event() {}

    void Window::set_content_widget(Widget* widget) noexcept {
        if (widget == nullptr) return;

        _content_widget = widget;
        _content_widget->_set_window(this);
    }

    void Window::_draw_widget(RolUI::Widget* widget, RolUI::IPainter* painter) noexcept {
        if (widget == nullptr || painter == nullptr) return;

        Point pos = widget->pos();

        widget->on_draw(painter);

        painter->push_pos(pos);

        for (auto& cw : widget->_children) {
            _draw_widget(cw, painter);
        }

        painter->pop_pos(pos);
    }
    void Window::draw() noexcept {
        Widget* root_widget = this->_content_widget;
        IPainter* painter = this->painter();

        if (root_widget == nullptr) return;
        if (painter == nullptr) return;

        begin_draw();
        _draw_widget(root_widget, painter);
        end_draw();
    }

    bool Window::send_event(Widget* w, IEvent* e) {
        if (w == nullptr || w->window() != this) return false;
        return w->do_event(e);
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
