
#include <unordered_set>

#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/events/MouseEvent.hpp"

namespace RolUI {

    RolUI_impl_event_type(MousePosEvent);
    RolUI_impl_event_type(MousePressEvent);
    RolUI_impl_event_type(MouseReleaseEvent);
    RolUI_impl_event_type(MouseEnterEvent);
    RolUI_impl_event_type(MouseLeaveEvent);
    RolUI_impl_event_type(MouseScrollEvent);

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

    void MouseDispatcher::set_scroll_offset(Vec2i scroll) noexcept {
        _is_scrolling = true;
        _scroll = scroll;
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
        _scroll = {0, 0};
    }

    void MouseDispatcher::dispatch(Window* w) noexcept {
        if (w == nullptr) return;

        Point mouse_pos = this->pos();

        Widget* widget = Application::get_widget_by_pos(mouse_pos);

        if (is_move() && !_is_enter && !_is_leave) {
            for (auto it = _hover_widgets.begin(); it != _hover_widgets.end();) {
                Widget* w = *it;
                if (w && w->hit_test(mouse_pos) == false) {
                    MouseEvent me = MouseEvent(MouseLeaveEvent_type(), w, this);
                    send_event(w, &me);
                    it = _hover_widgets.erase(it);
                } else
                    ++it;
            }

            Widget* tw = widget;
            while (tw && tw->hit_test(mouse_pos) && _hover_widgets.find(tw) == _hover_widgets.end()) {
                _hover_widgets.insert(tw);

                MouseEvent me = MouseEvent(MouseEnterEvent_type(), tw, this);
                send_event(tw, &me);

                tw = tw->parent();
            }
        } else if (_is_enter) {
            Widget* tw = widget;
            while (tw) {
                _hover_widgets.insert(tw);

                MouseEvent me = MouseEvent(MouseEnterEvent_type(), tw, this);
                send_event(tw, &me);

                tw = tw->parent();
            }
        } else if (_is_leave) {
            for (Widget* w : _hover_widgets) {
                // if (w && w->abs_rect().contain(mouse_pos) == false) {
                MouseEvent me = MouseEvent(MouseLeaveEvent_type(), w, this);
                send_event(w, &me);
                // }
            }
            _hover_widgets.clear();
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
                Widget* wi = Application::get_widget_by_pos(mouse_pos);
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

        if (is_scrolling()) {
            for (auto w : _hover_widgets) {
                MouseScrollEvent mse{w, scroll_offset()};
                send_event(w, &mse);
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
    Vec2i MouseEvent::offset() const noexcept {
        return _dispatcher->offset();
    }

    MouseKeyMode MouseEvent::button(MouseKey key) const noexcept {
        return _dispatcher->button(key);
    }
    MouseKey MouseEvent::action() const noexcept {
        return _action_key;
    }

    void MouseEvent::_set_action_key(MouseKey key) { _action_key = key; }

    MouseScrollEvent::MouseScrollEvent(Widget* target, Vec2i offset) noexcept
        : IEvent(MouseScrollEvent_type(), target), _offset(offset) {}

    Vec2i MouseScrollEvent::offset() const noexcept { return _offset; }

} // namespace RolUI
