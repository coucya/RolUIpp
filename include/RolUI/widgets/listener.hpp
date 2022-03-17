#pragma once

#include "../Widget.hpp"
#include "../sigslot/Signal.hpp"
#include "../events/MouseEvent.hpp"

namespace RolUI {
    namespace widgets {

        class PointerListenerWidget : public SingleChildWidget {
          public:
            Signal<Point> on_up;
            Signal<Point> on_down;
            Signal<Point> on_click;
            Signal<Vec2i> on_move;
            Signal<Vec2i> on_drag;
            Signal<bool> on_hover;

          public:
            PointerListenerWidget() noexcept;
            ~PointerListenerWidget() override;

            bool handle_event(IEvent* e) noexcept override;
            void draw(IPainter* painter) noexcept override;

          private:
            bool _is_press = false;
        };

        class MouseAreaWidget : public SingleChildWidget {
          public:
            Signal<MouseKey, Point> on_up;
            Signal<MouseKey, Point> on_down;
            Signal<MouseKey, Point> on_click;
            Signal<MouseKey, Vec2i> on_drag;
            Signal<Vec2i> on_move;
            Signal<Vec2i> on_wheel;
            Signal<bool> on_hover;

          public:
            MouseAreaWidget() noexcept;
            ~MouseAreaWidget() override;

            bool handle_event(IEvent* e) noexcept override;
            void draw(IPainter* painter) noexcept override;

          private:
            bool _is_press[MOUSE_KEY_COUNT] = {false};
        };

        class FocusWidget : public SingleChildWidget {
          public:
            Signal<bool> on_focus;

          public:
            FocusWidget() noexcept;

            void focus() noexcept;
            void unfocus() noexcept;

            bool handle_event(IEvent* e) noexcept override;
        };

        class CharInputWidget : public SingleChildWidget {
          public:
            Signal<uint32_t> on_input;

            CharInputWidget() noexcept;

            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI