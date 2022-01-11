
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
        if (!w || !w->parent()) return false;
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

    size_t Widget::child_count() const noexcept { return _children.size(); }
    Widget* Widget::get_child(size_t idx) const noexcept { return _get_child(idx); }

    void Widget::add_child(Widget* w) noexcept {
        _add_child(_child_end_it(), w);
    }
    void Widget::remove_child(Widget* w) noexcept {
        auto it = _find_child_it(w);
        _remove_child(it);
    }

    Widget* Widget::get_child_by_pos(Point pos) const noexcept {
        for (auto it = _child_rbegin_it(), ed = _child_rend_it(); it != ed; ++it) {
            Widget* w = *it;
            if (w->hit_test(pos - w->pos()))
                return w;
        }
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

    Size Widget::perlayout(Constraint constraint) {
        Size res = constraint.min;
        for (auto it = _child_begin_it(); it != _child_end_it(); ++it) {
            Widget* w = *it;
            Size size = w->perlayout(constraint);
            w->_pos = Point{0, 0};
            w->_size = size;
            res.width = std::max(res.width, size.width);
            res.height = std::max(res.height, size.height);
        }
        return res;
    }

    bool Widget::hit_test(Point local_pos) {
        return rect().contain(local_pos);
    }

    Widget* Widget::_get_child(size_t idx) const noexcept {
        return idx < _children.size() ? _children[idx] : nullptr;
    }
    void Widget::_add_child(Childrens::iterator pos, Widget* w) noexcept {
        if (!w || w->parent() == this) return;

        Widget* w_old_parent = w->parent();
        Widget* w_new_parent = this;

        if (w_old_parent) {
            auto it = w_old_parent->_find_child_it(w);
            w_old_parent->_remove_child(it);
        }

        _children.insert(pos, w);
        w->_parent = w_new_parent;
    }
    void Widget::_remove_child(Childrens::iterator pos) noexcept {
        if (pos == _children.end()) return;

        Widget* w = *pos;

        if (w->_parent != this) return;

        _children.erase(pos);
        w->_parent = nullptr;
    }

    auto Widget::_child_begin_it() const noexcept -> Childrens::iterator {
        return const_cast<Widget*>(this)->_children.begin();
    }
    auto Widget::_child_end_it() const noexcept -> Childrens::iterator {
        return const_cast<Widget*>(this)->_children.end();
    }
    auto Widget::_child_rbegin_it() const noexcept -> Childrens::reverse_iterator {
        return const_cast<Widget*>(this)->_children.rbegin();
    }
    auto Widget::_child_rend_it() const noexcept -> Childrens::reverse_iterator {
        return const_cast<Widget*>(this)->_children.rend();
    }
    auto Widget::_find_child_it(Widget* w) const noexcept -> Childrens::iterator {
        return std::find(_child_begin_it(), _child_end_it(), w);
    }

} // namespace RolUI
