
#include <unordered_set>

#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/events/MouseEvent.hpp"

namespace RolUI {

    RolUI_impl_object_type_of(MouseEvent, IEvent);
    RolUI_impl_object_type_of(MouseMoveEvent, MouseEvent);
    RolUI_impl_object_type_of(MousePressEvent, MouseEvent);
    RolUI_impl_object_type_of(MouseReleaseEvent, MouseEvent);
    RolUI_impl_object_type_of(MouseEnterEvent, MouseEvent);
    RolUI_impl_object_type_of(MouseLeaveEvent, MouseEvent);
    RolUI_impl_object_type_of(MouseWheelEvent, IEvent);

    MouseDispatcher::MouseDispatcher() noexcept {
        _last_pos = {};
        _current_pos = {};
        for (int i = 0; i < sizeof(_key_mode) / sizeof(MouseKeyMode); i++)
            _key_mode[i] = MouseKeyMode::release;
        clear_change();
    }

    bool MouseDispatcher::is_action(MouseKey key) const noexcept {
        return _key_is_change[(int)key];
    }

    MouseKeyMode MouseDispatcher::button(MouseKey key) const noexcept {
        return _key_mode[(int)key];
    }

    void MouseDispatcher::set_pos(Point pos) noexcept {
        if (pos == _current_pos) return;

        if (_pos_is_change) {
            _current_pos = pos;
        } else {
            _last_pos = _current_pos;
            _current_pos = pos;
        }
        _pos_is_change = true;
    }

    void MouseDispatcher::set_last_pos(Point pos) noexcept { _last_pos = pos; }

    void MouseDispatcher::set_wheel_offset(Vec2i scroll) noexcept {
        _is_scrolling = true;
        _scroll_offset = scroll;
    }

    void MouseDispatcher::set_key_mode(MouseKey key, MouseKeyMode mode) noexcept {
        int idx = (int)key;
        if (_key_mode[idx] != mode)
            _key_is_change[idx] = true;
        _key_mode[idx] = mode;
    }

    void MouseDispatcher::enter() noexcept {
        _is_enter = true;
    }
    void MouseDispatcher::leave() noexcept {
        _is_leave = true;
    }

    void MouseDispatcher::clear_change() noexcept {
        _pos_is_change = false;
        for (int i = 0; i < sizeof(_key_is_change) / sizeof(bool); i++) {
            _key_is_change[i] = false;
        }
        _is_enter = _is_leave = false;
        _is_scrolling = false;
        _scroll_offset = {0, 0};
    }

    void MouseDispatcher::_init_mouse_event(MouseEvent* e, MouseKey action_key) const noexcept {
        e->_mouse_pos = _current_pos;
        e->_mouse_offset = _current_pos - _last_pos;
        e->_action_key = action_key;
        for (int i = 0; i < sizeof(_key_is_change) / sizeof(bool); i++)
            e->_key_mode[i] = _key_mode[i];
    }

    void MouseDispatcher::dispatch(Window* w) noexcept {
        if (w == nullptr) return;

        Point mouse_pos = this->pos();
        Application::root_widget()->clear_hit();
        Application::root_widget()->hit_test(mouse_pos);
        visit_tree(Application::root_widget(), [this](Widget* w) {
            if (w->is_hit())
                this->_hit_widgets.insert(w);
            else
                this->_miss_widgets.insert(w);
        });

        if (is_move() && !_is_enter && !_is_leave) {
            for (Widget* w : _hit_widgets) {
                if (_last_hit_widgets.find(w) == _last_hit_widgets.end()) {
                    MouseEnterEvent me(w);
                    _init_mouse_event(&me);
                    send_event(w, &me);
                }
            }
            for (Widget* w : _miss_widgets) {
                if (_last_hit_widgets.find(w) != _last_hit_widgets.end()) {
                    MouseLeaveEvent me(w);
                    _init_mouse_event(&me);
                    send_event(w, &me);
                }
            }
        } else if (_is_enter) {
            for (Widget* w : _hit_widgets) {
                MouseEnterEvent me(w);
                _init_mouse_event(&me);
                send_event(w, &me);
            }
        } else if (_is_leave) {
            visit_tree(Application::root_widget(), [&, this](Widget* w) {
                if (this->_last_hit_widgets.find(w) != this->_last_hit_widgets.end()) {
                    MouseLeaveEvent me{w};
                    _init_mouse_event(&me);
                    send_event(w, &me);
                }
            });
            _hit_widgets.clear();
            _last_hit_widgets.clear();
        }

        if (is_move()) {
            for (Widget* w : _hit_widgets) {
                MouseMoveEvent me{w};
                _init_mouse_event(&me);
                send_event(w, &me);
            }
        }

        for (int i = 0; i < sizeof(_key_is_change) / sizeof(bool); i++) {
            if (is_action((MouseKey)i)) {
                for (Widget* w : _hit_widgets) {
                    MousePressEvent mpe{w};
                    MouseReleaseEvent mre{w};
                    MouseEvent* me = button((MouseKey)i) == MouseKeyMode::press
                                       ? (MouseEvent*)&mpe
                                       : (MouseEvent*)&mre;
                    _init_mouse_event(me, (MouseKey)i);
                    if (send_event(w, me)) break;
                }
            }
        }

        if (is_scrolling()) {
            for (auto w : _hit_widgets) {
                MouseWheelEvent mse{w, wheel_offset()};
                send_event(w, &mse);
            }
        }

        _last_hit_widgets = std::move(_hit_widgets);
        _hit_widgets.clear();
        _miss_widgets.clear();

        clear_change();
    }

    MouseEvent::MouseEvent(Widget* target) noexcept
        : IEvent(target) {
        _action_key = MouseKey::unkown;
    }

    Point MouseEvent::pos() const noexcept {
        return _mouse_pos;
    }
    Vec2i MouseEvent::offset() const noexcept {
        return _mouse_offset;
    }

    MouseKeyMode MouseEvent::button(MouseKey key) const noexcept {
        return _key_mode[(int)key];
    }
    MouseKey MouseEvent::action() const noexcept {
        return _action_key;
    }

    MouseWheelEvent::MouseWheelEvent(Widget* target, Vec2i offset) noexcept
        : IEvent(target), _offset(offset) {}

    Vec2i MouseWheelEvent::offset() const noexcept { return _offset; }

    MouseMoveEvent::MouseMoveEvent(Widget* target) noexcept : MouseEvent(target) {}
    MousePressEvent::MousePressEvent(Widget* target) noexcept : MouseEvent(target) {}
    MouseReleaseEvent::MouseReleaseEvent(Widget* target) noexcept : MouseEvent(target) {}
    MouseEnterEvent::MouseEnterEvent(Widget* target) noexcept : MouseEvent(target) {}
    MouseLeaveEvent::MouseLeaveEvent(Widget* target) noexcept : MouseEvent(target) {}

    const ObjectType* MouseEvent::object_type() const noexcept { return object_type_of<MouseEvent>(); }
    const ObjectType* MouseMoveEvent::object_type() const noexcept { return object_type_of<MouseMoveEvent>(); }
    const ObjectType* MousePressEvent::object_type() const noexcept { return object_type_of<MousePressEvent>(); }
    const ObjectType* MouseReleaseEvent::object_type() const noexcept { return object_type_of<MouseReleaseEvent>(); }
    const ObjectType* MouseEnterEvent::object_type() const noexcept { return object_type_of<MouseEnterEvent>(); }
    const ObjectType* MouseLeaveEvent::object_type() const noexcept { return object_type_of<MouseLeaveEvent>(); }
    const ObjectType* MouseWheelEvent::object_type() const noexcept { return object_type_of<MouseWheelEvent>(); }

} // namespace RolUI
