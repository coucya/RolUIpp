
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
        return w->do_event(e);
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

    size_t Widget::add_listener(const EventType* et, EventCallback&& callback) {
        _callbacks.push_back(std::make_tuple(et, std::move(callback), _event_handle));
        return _event_handle++;
    }
    void Widget::remove_listener(size_t handle) {
        _callbacks.erase(
            std::remove_if(
                std::begin(_callbacks), std::end(_callbacks),
                [=](const CallbackItem& cbi) { return std::get<2>(cbi) == handle; }),
            _callbacks.end());
    }

    bool Widget::on_event(IEvent* e) {
        bool b = false;
        for (const auto& [et, cb, handle] : _callbacks) {
            if (e->event_type() != et) continue;
            b = (cb ? cb(e) : false) || b;
        }
        return b;
    }
    void Widget::on_draw(IPainter* painter) {}

    Size Widget::perlayout(Constraint constraint) { return {0, 0}; }

    bool Widget::hit_test(Point local_pos) const {
        return RolUI::Rect{{0, 0}, size()}.contain(local_pos);
    }

} // namespace RolUI
