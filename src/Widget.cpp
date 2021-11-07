
#include <algorithm>
#include <vector>
#include <tuple>
#include <utility>

#include "RolUI/Application.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    bool send_event(Widget* w, IEvent* e) {
        if (!w || !w->window() || !w->window()->application()) return false;
        e->_target = w;
        return w->do_event(e);
    }

    Widget::Widget() noexcept {
        _init_event_bind();
    }

    Widget::Widget(Widget* parent) noexcept {
        _init_event_bind();
        parent->add_child(this);
    }

    Widget::~Widget() {}

    Point Widget::pos() const noexcept { return _pos; }
    Size Widget::size() const noexcept { return _size; }
    Rect Widget::rect() const noexcept { return {_pos, _size}; }

    Point Widget::abs_pos() const noexcept {
        if (_parent == nullptr) return _pos;
        return _parent->abs_pos() + _pos;
    }
    Rect Widget::abs_rect() const noexcept { return {abs_pos(), _size}; }

    void Widget::set_pos(const Point& pos) noexcept {
        Point old = _pos;
        _pos = pos;

        PosChangeEvent e{this, pos, old};
        send_event(this, &e);
    }
    void Widget::set_size(const Size& size) noexcept {
        Size old = _size;
        _size = size;

        SizeChangeEvent e{this, size, old};
        send_event(this, &e);
    }

    void Widget::set_pos(int32_t x, int32_t y) noexcept { set_pos({x, y}); }
    void Widget::set_size(uint32_t w, uint32_t h) noexcept { set_size({w, h}); }

    Widget* Widget::parent() const noexcept { return _parent; }

    Window* Widget::window() const noexcept { return _window; }

    bool Widget::is_child() const noexcept { return _parent && _index >= _parent->_part_count; }
    bool Widget::is_part() const noexcept { return _parent && _index < _parent->_part_count; }

    size_t Widget::child_index() const noexcept { return is_child() ? _index - _parent->_part_count : 0; }
    size_t Widget::part_index() const noexcept { return is_part() ? _index : 0; }

    void Widget::add_child(Widget* w) noexcept {
        _add_widget(_child_end_it(), w);
    }
    void Widget::remove_child(Widget* w) noexcept {
        auto it = _find_child_it(w);
        if (it == _child_end_it()) return;

        _remove_widget(it);
    }
    Widget* Widget::get_child(size_t idx) const noexcept {
        return _get_widget(_part_count + idx);
    }
    size_t Widget::child_count() const noexcept {
        return _children.size() - _part_count;
    }

    void Widget::add_part(Widget* w) noexcept {
        _add_widget(_part_end_it(), w);
        _part_count++;
    }
    void Widget::remove_part(Widget* w) noexcept {
        auto it = _find_part_it(w);
        if (it == _part_end_it()) return;

        _remove_widget(it);
        _part_count--;
    }
    Widget* Widget::get_part(size_t idx) const noexcept { return _get_widget(idx); }
    size_t Widget::part_count() const noexcept { return _part_count; }

    void Widget::remove_widget(Widget* w) noexcept {
        remove_part(w);
        remove_child(w);
    }

    Widget* Widget::get_child_by_pos(Point pos) const noexcept {
        for (auto it = _child_rbegin_it(), ed = _child_rend_it(); it != ed; ++it) {
            if ((*it)->rect().contain(pos))
                return *it;
        }
        return nullptr;
    }
    Widget* Widget::get_part_by_pos(Point pos) const noexcept {
        for (auto it = _part_rbegin_it(), ed = _part_rend_it(); it != ed; ++it) {
            if ((*it)->rect().contain(pos))
                return *it;
        }
        return nullptr;
    }
    Widget* Widget::get_widget_by_pos(Point pos) const noexcept {
        Widget* w = get_child_by_pos(pos);
        if (w) return w;
        return get_part_by_pos(pos);
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

    void Widget::_init_event_bind() noexcept {
        add_listener(PosChangeEvent::type(), [this](IEvent* e) {
            PosChangeEvent* pe = (PosChangeEvent*)e;
            this->on_pos_change.emit(pe->current_value());
            return true;
        });
        add_listener(SizeChangeEvent::type(), [this](IEvent* e) {
            SizeChangeEvent* se = (SizeChangeEvent*)e;
            this->on_size_change.emit(se->current_value());
            return true;
        });
    }
    void Widget::_update_child_index(size_t begin) noexcept {
        for (size_t i = begin; i < _children.size(); i++)
            _children[i]->_index = i;
    }

    Widget* Widget::_get_widget(size_t idx) const noexcept {
        return idx < _children.size() ? _children[idx] : nullptr;
    }
    void Widget::_add_widget(Childrens::iterator pos, Widget* w) noexcept {
        if (!w || w->parent() == this) return;

        if (w->_parent) {
            auto it = w->_parent->_find_widget_it(w);
            w->_parent->_children.erase(it);
        }

        w->_set_window(_window);
        w->_set_parent(this);

        size_t pos_idx = std::distance(_children.begin(), pos);

        _children.insert(pos, w);

        _update_child_index(pos_idx);
    }
    void Widget::_remove_widget(Childrens::iterator pos) noexcept {
        if (pos == _children.end()) return;

        Widget* w = *pos;
        w->_set_window(nullptr);
        w->_set_parent(nullptr);

        size_t pos_idx = std::distance(_children.begin(), pos);

        _children.erase(pos);

        _update_child_index(pos_idx);
    }

    void Widget::_set_window(Window* w) noexcept {
        if (_window == w) return;

        Window* old = _window;
        _window = w;

        if (w && w != old) {
            WindowChangeEvent e{this, w, old};
            send_event(this, &e);
        }

        for (auto& widget : _children)
            if (widget->_window != w)
                widget->_set_window(w);
    }

    void Widget::_set_parent(Widget* w) noexcept {
        if (_parent == w) return;

        Widget* old = _parent;
        _parent = w;

        if (w && w != old) {
            ParentChangeEvent e(this, w, old);
            send_event(this, &e);
        }
    }

    auto Widget::_child_begin_it() const noexcept -> Childrens::iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.begin() + _part_count;
    }
    auto Widget::_child_end_it() const noexcept -> Childrens::iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.end();
    }
    auto Widget::_part_begin_it() const noexcept -> Childrens::iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.begin();
    }
    auto Widget::_part_end_it() const noexcept -> Childrens::iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.begin() + _part_count;
    }

    auto Widget::_child_rbegin_it() const noexcept -> Childrens::reverse_iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.rbegin();
    }
    auto Widget::_child_rend_it() const noexcept -> Childrens::reverse_iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.rbegin() + (_children.size() - _part_count);
    }
    auto Widget::_part_rbegin_it() const noexcept -> Childrens::reverse_iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.rbegin() + (_children.size() - _part_count);
    }
    auto Widget::_part_rend_it() const noexcept -> Childrens::reverse_iterator {
        Widget* _this = const_cast<Widget*>(this);
        return _this->_children.rend();
    }

    auto Widget::_find_widget_it(Widget* w) const noexcept -> Childrens::iterator {
        Widget* _this = const_cast<Widget*>(this);
        auto b = _this->_children.begin();
        auto e = _this->_children.end();
        return std::find(b, e, w);
    }
    auto Widget::_find_child_it(Widget* w) const noexcept -> Childrens::iterator {
        return std::find(_child_begin_it(), _child_end_it(), w);
    }
    auto Widget::_find_part_it(Widget* w) const noexcept -> Childrens::iterator {
        return std::find(_part_begin_it(), _part_end_it(), w);
    }

} // namespace RolUI
