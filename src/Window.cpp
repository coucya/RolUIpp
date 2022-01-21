
#include <climits>

#include "RolUI/IEvent.hpp"
#include "RolUI/WidgetState.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    Window::Window() noexcept {
        _init_event_bind();
    }
    Window::~Window() {}

    void Window::_init_event_bind() noexcept {}

    IPainter* Window::painter() { return nullptr; }
    void Window::begin_draw() {}
    void Window::end_draw() {}

    void Window::dispatch_event(double timeout) {}

    Widget* Window::content_widget() const noexcept { return _content_widget; }
    void Window::set_content_widget(Widget* widget) noexcept {
        if (widget == nullptr) return;
        _content_widget = widget;
    }

    void Window::_draw_widget(RolUI::Widget* widget, Point base_pos, Rect scissor, RolUI::IPainter* painter) noexcept {
        if (widget == nullptr || painter == nullptr) return;

        Point pos = widget->pos();
        Size size = widget->size();
        Point new_base_pos = base_pos + pos;
        auto new_scissor_opt = scissor.intersected({new_base_pos, size});
        Rect new_scissor = new_scissor_opt.has_value() ? new_scissor_opt.value() : Rect{scissor.pos(), Size{0, 0}};

        painter->set_base_pos(new_base_pos);
        painter->scissor(new_scissor);

        widget->on_draw(painter);

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
        if (_content_widget == nullptr) return nullptr;

        Widget* widget = nullptr;
        Widget* w_it = _content_widget;
        Point pos_it = pos - _content_widget->pos();

        if (w_it->hit_test(pos_it) == false) return nullptr;

        while (true) {
            Widget* w = w_it->get_child_by_pos(pos_it);
            if (w == nullptr) return w_it;
            w_it = w;
            pos_it = pos_it - w_it->pos();
        }
        return widget;
    }

} // namespace RolUI
