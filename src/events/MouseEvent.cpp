
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/MouseEvent.hpp"

namespace RolUI {

    RolUI_impl_event_type(MousePosEvent);
    RolUI_impl_event_type(MousePressEvent);
    RolUI_impl_event_type(MouseReleaseEvent);
    RolUI_impl_event_type(MouseEnterEvent);
    RolUI_impl_event_type(MouseLeaveEvent);

    MouseDispatcher::MouseDispatcher() noexcept {
        _init();
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
    void MouseDispatcher::set_pos(int32_t x, int32_t y) noexcept {
        set_pos({x, y});
    }

    void MouseDispatcher::set_last_pos(Point pos) noexcept { _last_pos = pos; }
    void MouseDispatcher::set_last_pos(int32_t x, int32_t y) noexcept {
        set_last_pos({x, y});
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
    }

    void MouseDispatcher::dispatch(Window* w) noexcept {
        if (w == nullptr) return;

        Point mouse_pos = this->pos();

        Widget* widget = w->get_widget_by_pos(mouse_pos);

        if (is_move() && !_is_enter && !_is_leave) {
            Widget* tw = _current_widget;
            while (tw && tw->visual_rect().contain(mouse_pos) == false) {
                MouseEvent me = MouseEvent(MouseLeaveEvent_type(), tw, this);
                send_event(tw, &me);

                tw = tw->parent();
            }

            tw = widget;
            while (tw && tw->visual_rect().contain(_last_pos) == false) {
                MouseEvent me = MouseEvent(MouseEnterEvent_type(), tw, this);
                send_event(tw, &me);

                tw = tw->parent();
            }
            _current_widget = widget;
        } else if (_is_enter) {
            Widget* tw = widget;
            while (tw) {
                MouseEvent me = MouseEvent(MouseEnterEvent_type(), tw, this);
                send_event(tw, &me);

                tw = tw->parent();
            }
            _current_widget = widget;
        } else if (_is_leave) {
            Widget* tw = _current_widget;
            while (tw) {
                MouseEvent me = MouseEvent(MouseLeaveEvent_type(), tw, this);
                send_event(tw, &me);

                tw = tw->parent();
            }
            _current_widget = nullptr;
        }

        if (is_move()) {
            Widget* tw = widget;
            while (tw) {
                MouseEvent me = MouseEvent(MousePosEvent_type(), tw, this);
                me._set_action_key(MouseKey::unkown);

                if (send_event(tw, &me)) break;

                tw = tw->parent();
            }
        }

        for (int i = 0; i < sizeof(_key_is_change); i++) {
            if (is_action((MouseKey)i)) {
                Widget* tw = widget;
                while (tw) {
                    const EventType* et = button((MouseKey)i) == MouseKeyMode::press
                                            ? MousePressEvent_type()
                                            : MouseReleaseEvent_type();
                    MouseEvent me = MouseEvent(et, tw, this);
                    me._set_action_key((MouseKey)i);

                    if (send_event(tw, &me)) break;

                    tw = tw->parent();
                }
            }
        }
    }

    MouseEvent::MouseEvent(const EventType* et, Widget* target, const MouseDispatcher* dispatcher) noexcept
        : IEvent(et, target), _dispatcher(dispatcher) {
        _action_key = MouseKey::unkown;
    }

    Point MouseEvent::pos() const noexcept {
        return _dispatcher->pos();
    }
    Vector MouseEvent::offset() const noexcept {
        return _dispatcher->offset();
    }

    MouseKeyMode MouseEvent::button(MouseKey key) const noexcept {
        return _dispatcher->button(key);
    }
    MouseKey MouseEvent::action() const noexcept {
        return _action_key;
    }

    void MouseEvent::_set_action_key(MouseKey key) { _action_key = key; }

} // namespace RolUI
