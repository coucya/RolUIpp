
#include <algorithm>
#include <cstddef>
#include <cstdint>
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
        if (parent) parent->add_child(this);
    }

    Widget::~Widget() {}

    Point Widget::pos() const noexcept {
        Point target_base_pos = {0, 0};
        Widget* target = nullptr;
        switch (_pos_relative) {
            case RelativeTarget::parent: target = parent(); break;
            case RelativeTarget::prev: {
                if (is_part() && part_index() > 0)
                    target = parent()->get_part(part_index() - 1);
                else if (is_child() && child_index() > 0)
                    target = parent()->get_child(child_index() - 1);
                else
                    target = nullptr;
                break;
            }
            case RelativeTarget::target: target = _pos_target; break;
        };

        if (target) {
            if (target != parent())
                target_base_pos = target->pos();
            else
                target_base_pos = {0, 0};

            switch (_target_anchor_point) {
                case AnchorPoint::left_top: {
                    target_base_pos += {};
                    break;
                }
                case AnchorPoint::left_bottom: {
                    Size s = target->size();
                    target_base_pos += {0, (int32_t)s.height};
                    break;
                }
                case AnchorPoint::right_top: {
                    Size s = target->size();
                    target_base_pos += {(int32_t)s.width, 0};
                    break;
                }
                case AnchorPoint::right_bottom: {
                    Size s = target->size();
                    target_base_pos += {(int32_t)s.width, (int32_t)s.height};
                    break;
                }
            }
        }

        Point self_base_pos = {0, 0};
        switch (_self_anchor_point) {
            case AnchorPoint::left_top: {
                self_base_pos = {};
                break;
            }
            case AnchorPoint::left_bottom: {
                Size s = size();
                self_base_pos = {0, (int32_t)s.height};
                break;
            }
            case AnchorPoint::right_top: {
                Size s = size();
                self_base_pos = {(int32_t)s.width, 0};
                break;
            }
            case AnchorPoint::right_bottom: {
                Size s = size();
                self_base_pos = {(int32_t)s.width, (int32_t)s.height};
                break;
            }
        };

        return target_base_pos - self_base_pos + _pos;
    }

    Size Widget::size() const noexcept {

        Widget* target = nullptr;
        switch (_size_relative) {
            case RelativeTarget::parent: target = parent(); break;
            case RelativeTarget::prev: {
                if (is_part() && part_index() > 0)
                    target = parent()->get_part(part_index() - 1);
                else if (is_child() && child_index() > 0)
                    target = parent()->get_child(child_index() - 1);
                else
                    target = nullptr;
                break;
            }
            case RelativeTarget::target: target = _size_target; break;
        };
        Size target_size = target ? target->size() : Size();

        Size res;
        switch (_size_mode) {
            case SizeMode::none: {
                res = _size;
                break;
            }
            case SizeMode::relative: {
                res = target_size + _size;
                break;
            }
        };
        return res;
    }
    Rect Widget::rect() const noexcept { return {pos(), size()}; }

    Point Widget::abs_pos() const noexcept {
        if (_parent == nullptr) return pos();
        return _parent->abs_pos() + pos();
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

    void Widget::set_pos(int x, int y) noexcept { set_pos({x, y}); }
    void Widget::set_size(int w, int h) noexcept { set_size({w, h}); }

    void Widget::set_pos_target(Widget* target) noexcept {
        if (target && target->parent() && target->parent() == parent())
            _pos_target = target;
        else
            _pos_target = nullptr;
    }
    void Widget::set_pos_relative(RelativeTarget relative, AnchorPoint target, AnchorPoint self) noexcept {
        _pos_relative = relative;
        _target_anchor_point = target;
        _self_anchor_point = self;
    }
    void Widget::set_size_target(Widget* target) noexcept {
        if (target && target->parent() && target->parent() == parent())
            _size_target = target;
        else
            _size_target = nullptr;
    }
    void Widget::set_size_relative(RelativeTarget relative, SizeMode size_mode) noexcept {
        _size_relative = relative;
        _size_mode = size_mode;
    }

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
    void Widget::on_draw(IPainter* painter) {}

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

        Widget* w_old_parent = w->parent();
        Widget* w_new_parent = this;

        if (w_old_parent) {
            auto it = w_old_parent->_find_widget_it(w);
            w_old_parent->_children.erase(it);
            w->_do_detached_tree(w_old_parent, w->_index);
        }

        size_t pos_idx = std::distance(_children.begin(), pos);
        _children.insert(pos, w);

        w->_do_attach_tree(w_new_parent, pos_idx);
        w->_do_window_change(w_old_parent ? w_old_parent->window() : nullptr, w_new_parent->window());
        w->_do_parent_change(w_old_parent, w_new_parent);
    }
    void Widget::_remove_widget(Childrens::iterator pos) noexcept {
        if (pos == _children.end()) return;

        Widget* w = *pos;

        if (w->_parent != this) return;

        _children.erase(pos);

        size_t pos_idx = std::distance(_children.begin(), pos);
        w->_do_detached_tree(this, pos_idx);
        w->_do_window_change(_window, nullptr);
        w->_do_parent_change(this, nullptr);
    }

    void Widget::_do_attach_tree(Widget* new_parent, size_t idx) noexcept {
        if (!new_parent) return;
        new_parent->_update_child_index(idx);
    }
    void Widget::_do_detached_tree(Widget* old_parent, size_t idx) noexcept {
        if (!old_parent) return;

        for (Widget* widget : old_parent->_children) {
            if (widget->_pos_target == this)
                widget->_pos_target = nullptr;
        }

        old_parent->_update_child_index(idx);
    }

    void Widget::_do_parent_change(Widget* old, Widget* new_) noexcept {
        _parent = new_;

        if (old == new_ || !new_) return;

        ParentChangeEvent e(this, new_, old);
        send_event(this, &e);
    }
    void Widget::_do_window_change(Window* old, Window* new_) noexcept {
        _window = new_;
        for (auto& widget : _children)
            widget->_do_window_change(old, new_);

        if (old == new_ || !new_) return;

        WindowChangeEvent e{this, new_, old};
        send_event(this, &e);
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
