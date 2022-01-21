
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <tuple>
#include <utility>

#include "RolUI/Application.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    bool send_event(Widget* w, IEvent* e) {
        if (!w) return false;
        e->_target = w;
        return w->handle_event(e);
    }

    Size perlayout(Widget* w, Constraint constraint) {
        return w->perlayout(constraint);
    }
    void set_rect(Widget* w, Rect rect) {
        if (!w) return;
        w->_pos = rect.pos();
        w->_size = rect.size();
    }

    Widget::Widget() noexcept {}

    Widget::~Widget() {}

    Point Widget::pos() const noexcept { return _pos; }
    Size Widget::size() const noexcept { return _size; }

    Rect Widget::rect() const noexcept { return {pos(), size()}; }

    Point Widget::abs_pos() const noexcept {
        return parent() ? parent()->abs_pos() + pos() : pos();
    }
    Rect Widget::abs_rect() const noexcept { return {abs_pos(), size()}; }

    Widget* Widget::parent() const noexcept { return _parent; }

    Widget* Widget::get_child_by_pos(Point pos) const noexcept {
        return nullptr;
    }

    bool Widget::handle_event(IEvent* e) { return false; }
    void Widget::on_draw(IPainter* painter) {}

    Size Widget::perlayout(Constraint constraint) { return {0, 0}; }

    bool Widget::hit_test(Point local_pos) const {
        return RolUI::Rect{{0, 0}, size()}.contain(local_pos);
    }

} // namespace RolUI
