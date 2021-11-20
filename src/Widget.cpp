
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
#include "RolUI/WidgetState.hpp"
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

    Point Widget::pos() const noexcept { return _real_pos; }
    Size Widget::size() const noexcept { return _real_size; }

    Size Widget::visual_size() const noexcept {
        return visual_rect().size();
    }

    Point Widget::real_pos() const noexcept { return _real_pos; }
    Point Widget::rela_pos() const noexcept { return _rela_pos; }

    Rect Widget::rect() const noexcept { return {pos(), size()}; }

    Point Widget::abs_pos() const noexcept {
        if (_parent == nullptr) return pos();
        return _parent->abs_pos() + pos();
    }
    Rect Widget::abs_rect() const noexcept { return {abs_pos(), size()}; }

    Rect Widget::visual_rect() const noexcept {
        if (!parent()) return Rect(abs_pos(), size());
        Rect self_abs_rect = abs_rect();
        auto opt = parent()->visual_rect().intersected(self_abs_rect);
        return opt.has_value() ? opt.value() : Rect(self_abs_rect.pos(), Size(0, 0));
    }

    void Widget::set_pos(const Point& pos) noexcept {
        _rela_pos = pos;
        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }
    void Widget::set_size(const Size& size) noexcept {
        _rela_size = size;
        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }

    void Widget::set_pos(int x, int y) noexcept { set_pos({x, y}); }
    void Widget::set_size(int w, int h) noexcept { set_size({w, h}); }

    void Widget::move(int x, int y) noexcept { move({x, y}); }
    void Widget::move(Point offset) noexcept {
        _rela_pos += offset;
        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }

    void Widget::set_pos_target(Widget* target) noexcept {
        if (target && target->parent() && target->parent() == parent())
            _pos_target = target;
        else
            _pos_target = nullptr;

        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }
    void Widget::set_pos_relative(RelativeTarget relative, AnchorPoint target, AnchorPoint self) noexcept {
        _pos_relative = relative;
        _target_anchor_point = target;
        _self_anchor_point = self;

        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }
    void Widget::set_size_target(Widget* target) noexcept {
        if (target && target->parent() && target->parent() == parent())
            _size_target = target;
        else
            _size_target = nullptr;

        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }
    void Widget::set_size_relative(RelativeTarget relative, SizeMode size_mode) noexcept {
        _size_relative = relative;
        _size_mode = size_mode;

        if (parent())
            parent()->_update_child_size_and_pos();
        else
            _update_size_and_pos();
    }

    Point Widget::anchor_point() const noexcept {
        return anchor_point(_self_anchor_point);
    }
    Point Widget::anchor_point(AnchorPoint anchor_point) const noexcept {
        Rect self_rect = Rect({0, 0}, size());
        Point point;
        switch (anchor_point) {
            case AnchorPoint::left_top: point = self_rect.left_top(); break;
            case AnchorPoint::left_bottom: point = self_rect.left_bottom(); break;
            case AnchorPoint::right_top: point = self_rect.right_top(); break;
            case AnchorPoint::right_bottom: point = self_rect.right_bottom(); break;
            case AnchorPoint::centre_top: point = self_rect.centre_top(); break;
            case AnchorPoint::centre_bottom: point = self_rect.centre_bottom(); break;
            case AnchorPoint::left_middle: point = self_rect.left_middle(); break;
            case AnchorPoint::right_middle: point = self_rect.right_middle(); break;
            case AnchorPoint::centre_middle: point = self_rect.centre_middle(); break;
        }
        return point;
    }

    Widget* Widget::pos_target() const noexcept { return _pos_target; }
    Widget* Widget::size_target() const noexcept { return _size_target; }

    AnchorPoint Widget::self_anchor_point() const noexcept { return _self_anchor_point; }
    AnchorPoint Widget::target_anchor_point() const noexcept { return _target_anchor_point; }
    SizeMode Widget::size_mode() const noexcept { return _size_mode; }

    WidgetState Widget::state() const noexcept { return _widget_state; }

    bool Widget::is_enable() const noexcept { return !widget_state_has_state(_widget_state, WIDGET_STATE_DISABLED); };
    void Widget::set_enable(bool b) noexcept {
        b ? _clear_state(WIDGET_STATE_DISABLED) : _set_state(WIDGET_STATE_DISABLED);
    }

    bool Widget::focusable() const noexcept { return _focusable; }
    void Widget::set_focusable(bool b) noexcept { _focusable = b; }

    bool Widget::is_focus() const noexcept {
        if (window())
            return widget_state_has_state(_widget_state, WIDGET_STATE_FOCUS)
                && window()->focus_widget() == this;
        return false;
    }
    void Widget::set_focus() noexcept {
        if (window()) window()->set_focus_widget(this);
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
            return this->on_pos_change.slot_count() > 0;
        });
        add_listener(SizeChangeEvent::type(), [this](IEvent* e) {
            SizeChangeEvent* se = (SizeChangeEvent*)e;
            this->on_size_change.emit(se->current_value());
            return this->on_size_change.slot_count() > 0;
        });
        add_listener(FocusChangeEvent::type(), [this](IEvent* e) {
            FocusChangeEvent* fe = (FocusChangeEvent*)e;
            if (fe->current_value()) {
                this->on_focus.emit();
                return this->on_size_change.slot_count() > 0;
            } else {
                return false;
            }
        });
    }

    Widget* Widget::_prev_widget() noexcept {
        if (is_part() && part_index() > 0)
            return parent()->get_part(part_index() - 1);
        else if (is_child() && child_index() > 0)
            return parent()->get_child(child_index() - 1);
        else
            return nullptr;
    }

    void Widget::_update_child_index(size_t begin) noexcept {
        for (size_t i = begin; i < _children.size(); i++)
            _children[i]->_index = i;
    }

    void Widget::_update_size() noexcept {
        Size target_size = {0, 0};
        switch (_size_relative) {
            case RelativeTarget::parent: {
                if (parent())
                    target_size = parent()->size();
                else if (!parent() && window())
                    target_size = window()->size();
                break;
            }
            case RelativeTarget::prev: target_size = _prev_widget() ? _prev_widget()->size() : target_size; break;
            case RelativeTarget::target: target_size = _size_target ? _size_target->size() : target_size; break;
        };

        Size res;
        switch (_size_mode) {
            case SizeMode::none: res = _rela_size; break;
            case SizeMode::relative: res = target_size + _rela_size; break;
        };
        _real_size = res;
    }
    void Widget::_update_pos() noexcept {
        Widget* target = nullptr;
        Rect target_rect = Rect{{0, 0}, {0, 0}};
        switch (_pos_relative) {
            case RelativeTarget::parent: {
                if (parent())
                    target_rect = Rect{{0, 0}, parent()->size()};
                else if (!parent() && window())
                    target_rect = Rect{{0, 0}, window()->size()};
                break;
            }
            case RelativeTarget::prev: target_rect = _prev_widget() ? _prev_widget()->rect() : target_rect; break;
            case RelativeTarget::target: target_rect = _pos_target ? _pos_target->rect() : target_rect; break;
        };

        Point target_base_pos = {0, 0};
        switch (_target_anchor_point) {
            case AnchorPoint::left_top: target_base_pos = target_rect.left_top(); break;
            case AnchorPoint::left_bottom: target_base_pos = target_rect.left_bottom(); break;
            case AnchorPoint::right_top: target_base_pos = target_rect.right_top(); break;
            case AnchorPoint::right_bottom: target_base_pos = target_rect.right_bottom(); break;
            case AnchorPoint::centre_top: target_base_pos = target_rect.centre_top(); break;
            case AnchorPoint::centre_bottom: target_base_pos = target_rect.centre_bottom(); break;
            case AnchorPoint::left_middle: target_base_pos = target_rect.left_middle(); break;
            case AnchorPoint::right_middle: target_base_pos = target_rect.right_middle(); break;
            case AnchorPoint::centre_middle: target_base_pos = target_rect.centre_middle(); break;
        }

        Point self_base_pos = this->anchor_point();

        _real_pos = target_base_pos - self_base_pos + _rela_pos;
    }
    bool Widget::_update_size_and_pos() noexcept {

        Size ols_size = size();
        Point old_pos = pos();

        if (_size_mode == SizeMode::none) {
            _update_size();
        } else if (_size_relative == RelativeTarget::parent) {
            _update_size();
        } else if (_pos_relative == RelativeTarget::prev) {
            _update_size();
        } else if (_pos_relative == RelativeTarget::target) {
            if (_size_target && _size_target->_is_update == false)
                _update_size_and_pos();
            _update_size();
        }

        if (_pos_relative == RelativeTarget::parent) {
            _update_pos();
        } else if (_pos_relative == RelativeTarget::prev) {
            _update_pos();
        } else if (_pos_relative == RelativeTarget::target) {
            if (_pos_target && _pos_target->_is_update == false)
                _update_size_and_pos();
            _update_pos();
        }

        bool is_change = false;
        if (ols_size != size()) {
            is_change = true;
            SizeChangeEvent e = SizeChangeEvent(this, size(), ols_size);
            send_event(this, &e);
        }
        if (old_pos != pos()) {
            is_change = true;
            PosChangeEvent e = PosChangeEvent(this, pos(), old_pos);
            send_event(this, &e);
        }
        return is_change;
    }
    void Widget::_update_child_size_and_pos() noexcept {
        for (Widget* w : _children)
            w->_is_update = false;

        for (Widget* w : _children) {
            if (w->_is_update) continue;
            if (w->_update_size_and_pos())
                w->_update_child_size_and_pos();
            w->_is_update = true;
        }
    }

    void Widget::_set_state(WidgetState s) noexcept {
        WidgetState old = _widget_state;
        _widget_state = widget_state_set_state(_widget_state, s);
        WidgetState new_ = _widget_state;

        StateChangeEvent e{this, new_, old};
        send_event(this, &e);
    }

    void Widget::_clear_state(WidgetState s) noexcept {
        WidgetState old = _widget_state;
        _widget_state = widget_state_clear_state(_widget_state, s);
        WidgetState new_ = _widget_state;

        StateChangeEvent e{this, new_, old};
        send_event(this, &e);
    }
    void Widget::_reset_state() noexcept {
        WidgetState old = _widget_state;
        _widget_state = WIDGET_STATE_DEFAULT;
        WidgetState new_ = _widget_state;

        StateChangeEvent e{this, new_, old};
        send_event(this, &e);
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
        new_parent->_update_child_size_and_pos();
    }
    void Widget::_do_detached_tree(Widget* old_parent, size_t idx) noexcept {
        if (!old_parent) return;

        for (Widget* widget : old_parent->_children) {
            if (widget->_size_target == this)
                widget->_size_target = nullptr;
            if (widget->_pos_target == this)
                widget->_pos_target = nullptr;
        }

        old_parent->_update_child_index(idx);
        old_parent->_update_child_size_and_pos();
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
