#pragma once

#include <stdint.h>
#include <unordered_set>

#include "../Point.hpp"
#include "../Vector.hpp"
#include "../IEvent.hpp"
#include "../Window.hpp"

namespace RolUI {

    class Widget;
    class MouseDispatcher;

    enum class MouseKeyMode {
        press,
        release,
    };

    enum class MouseKey : char {
        unkown = -1,
        key1 = 0,
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

      protected:
        MouseEvent(Widget* target, const MouseDispatcher* dispatcher) noexcept;

      public:
        const ObjectType* object_type() const noexcept override;

        Point pos() const noexcept;
        Vec2i offset() const noexcept;

        MouseKey action() const noexcept;
        MouseKeyMode button(MouseKey key) const noexcept;

      private:
        void _set_action_key(MouseKey key);

      private:
        MouseKey _action_key;
        const MouseDispatcher* _dispatcher;
    };

    class MouseWheelEvent : public IEvent {
        friend class MouseDispatcher;

      public:
        MouseWheelEvent(Widget* target, Vec2i offset) noexcept;

        const ObjectType* object_type() const noexcept override;

        Vec2i offset() const noexcept;

      private:
        Vec2i _offset;
    };

    class MouseMoveEvent : public MouseEvent {
      public:
        MouseMoveEvent(Widget* target, const MouseDispatcher* dispatcher) noexcept;
        const ObjectType* object_type() const noexcept override;
    };
    class MousePressEvent : public MouseEvent {
      public:
        MousePressEvent(Widget* target, const MouseDispatcher* dispatcher) noexcept;
        const ObjectType* object_type() const noexcept override;
    };
    class MouseReleaseEvent : public MouseEvent {
      public:
        MouseReleaseEvent(Widget* target, const MouseDispatcher* dispatcher) noexcept;
        const ObjectType* object_type() const noexcept override;
    };
    class MouseEnterEvent : public MouseEvent {
      public:
        MouseEnterEvent(Widget* target, const MouseDispatcher* dispatcher) noexcept;
        const ObjectType* object_type() const noexcept override;
    };
    class MouseLeaveEvent : public MouseEvent {
      public:
        MouseLeaveEvent(Widget* target, const MouseDispatcher* dispatcher) noexcept;
        const ObjectType* object_type() const noexcept override;
    };

    RolUI_decl_object_type_of(MouseEvent);
    RolUI_decl_object_type_of(MouseMoveEvent);
    RolUI_decl_object_type_of(MousePressEvent);
    RolUI_decl_object_type_of(MouseReleaseEvent);
    RolUI_decl_object_type_of(MouseEnterEvent);
    RolUI_decl_object_type_of(MouseLeaveEvent);
    RolUI_decl_object_type_of(MouseWheelEvent);

    class MouseDispatcher {
      public:
        MouseDispatcher() noexcept;

        Point pos() const noexcept { return _current_pos; }
        Vec2i offset() const noexcept { return _current_pos - _last_pos; }

        MouseKeyMode button(MouseKey key) const noexcept;

        Vec2i wheel() const noexcept { return _scroll; }

        bool is_pos_change() const noexcept { return _pos_is_change; }
        bool is_move() const noexcept { return _pos_is_change; }

        bool is_action(MouseKey key) const noexcept;

        bool is_scrolling() const noexcept { return _is_scrolling; }

        void set_pos(Point pos) noexcept;
        void set_pos(int32_t x, int32_t y) noexcept;

        void set_last_pos(Point pos) noexcept;
        void set_last_pos(int32_t x, int32_t y) noexcept;

        void set_wheel(Vec2i scroll) noexcept;

        void set_key_mode(MouseKey key, MouseKeyMode mode) noexcept;

        void enter() noexcept;
        void leave() noexcept;

        void clear_change() noexcept;

        void dispatch(Window* w) noexcept;

      private:
        void _init() noexcept {
            _last_pos = {};
            _current_pos = {};
            for (int i = 0; i < sizeof(_key_mode) / sizeof(MouseKeyMode); i++)
                _key_mode[i] = MouseKeyMode::release;
        }

      private:
        bool _pos_is_change;
        Point _last_pos;
        Point _current_pos;

        bool _key_is_change[MOUSE_KEY_COUNT];
        MouseKeyMode _key_mode[MOUSE_KEY_COUNT];

        bool _is_scrolling = false;
        Vec2i _scroll;

        bool _is_enter = false;
        bool _is_leave = false;

        std::unordered_set<Widget*> _hover_widgets;
    };

} // namespace RolUI
