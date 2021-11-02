#pragma once

#include <stdint.h>

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Window.hpp"

namespace RolUI {

    class Widget;
    class MouseDispatcher;

    enum class MouseKeyMode {
        press,
        release,
    };

    enum class MouseKey : char {
        unkown = -1,
        key1 = 1,
        key2,
        key3,
        key4,
        key5,
        key6,
        key7,
        key8,

        left = key1,
        right = key2,
        middle = key3,
    };

    constexpr int MOUSE_KEY_COUNT = 8;

    class MouseEvent : public IEvent {
        friend class MouseDispatcher;

      private:
        MouseEvent(const EventType* et, Widget* target, const MouseDispatcher* dispatcher) noexcept;

      public:
        Point pos() const noexcept;
        Vector offset() const noexcept;

        MouseKey action() const noexcept;
        MouseKeyMode button(MouseKey key) const noexcept;

      private:
        void _set_action_key(MouseKey key);

      private:
        MouseKey _action_key;
        const MouseDispatcher* _dispatcher;
    };

    RolUI_decl_event_type(MousePosEvent);
    RolUI_decl_event_type(MouseKeyEvent);

    class MouseDispatcher {
      public:
        Point pos() const noexcept { return _current_pos; }
        Vector offset() const noexcept { return _current_pos - _last_pos; }

        MouseKeyMode button(MouseKey key) const noexcept;

        bool is_pos_change() const noexcept { return _pos_is_change; }
        bool is_move() const noexcept { return _pos_is_change; }

        bool is_action(MouseKey key) const noexcept;

        void set_pos(Point pos) noexcept;
        void set_pos(int32_t x, int32_t y) noexcept;

        void set_last_pos(Point pos) noexcept;
        void set_last_pos(int32_t x, int32_t y) noexcept;

        void set_key_mode(MouseKey key, MouseKeyMode mode) noexcept;

        void clear_change() noexcept;

        void dispatch(Window* w) noexcept;

      private:
        bool _pos_is_change;
        Point _last_pos;
        Point _current_pos;

        bool _key_is_change[MOUSE_KEY_COUNT + 1];
        MouseKeyMode _key_mode[MOUSE_KEY_COUNT + 1];
    };

} // namespace RolUI
