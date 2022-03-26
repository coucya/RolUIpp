#pragma once

#include "../Widget.hpp"
#include "../sigslot/Signal.hpp"
#include "../events/MouseEvent.hpp"
#include "../events/KeyboardEvent.hpp"

namespace RolUI {
    namespace widgets {

        class PointerListener : public SingleChildWidget {
          public:
            Signal<Point> on_up;
            Signal<Point> on_down;
            Signal<Point> on_click;
            Signal<Vec2i> on_move;
            Signal<Vec2i> on_drag;
            Signal<bool> on_hover;

          public:
            PointerListener() noexcept;
            ~PointerListener() override;

            bool handle_event(IEvent* e) noexcept override;
            void draw(IPainter* painter) noexcept override;

          private:
            bool _is_press = false;
        };

        class MouseListener : public SingleChildWidget {
          public:
            Signal<MouseKey, Point> on_up;
            Signal<MouseKey, Point> on_down;
            Signal<MouseKey, Point> on_click;
            Signal<MouseKey, Vec2i> on_drag;
            Signal<Vec2i> on_move;
            Signal<Vec2i> on_wheel;
            Signal<bool> on_hover;

          public:
            MouseListener() noexcept;
            ~MouseListener() override;

            bool handle_event(IEvent* e) noexcept override;
            void draw(IPainter* painter) noexcept override;

          private:
            bool _is_press[MOUSE_KEY_COUNT] = {false};
        };

        class KeyboardListener : public SingleChildWidget {
          public:
            Signal<KeyboardKey, KeyboardKeyMode> on_key;

          public:
            KeyboardListener() noexcept;
            ~KeyboardListener() override;

            bool handle_event(IEvent* e) noexcept override;
        };

        class FocusListener : public SingleChildWidget {
          public:
            Signal<bool> on_focus;

          public:
            FocusListener() noexcept;

            void focus() noexcept;
            void unfocus() noexcept;

            bool handle_event(IEvent* e) noexcept override;
        };

        class CharInputListener : public SingleChildWidget {
          public:
            Signal<uint32_t> on_input;

            CharInputListener() noexcept;

            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI