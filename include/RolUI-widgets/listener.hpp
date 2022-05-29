#pragma once

#include "RolUI/Widget.hpp"
#include "RolUI/sigslot.hpp"
#include "RolUI-events/MouseEvent.hpp"
#include "RolUI-events/KeyboardEvent.hpp"

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

            const ObjectType* object_type() const noexcept override;

          protected:
            bool handle_event(IEvent* e) noexcept override;

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

            const ObjectType* object_type() const noexcept override;

          protected:
            bool handle_event(IEvent* e) noexcept override;

          private:
            bool _is_press[MOUSE_KEY_COUNT] = {false};
        };

        class KeyboardListener : public SingleChildWidget {
          public:
            Signal<KeyboardKey, KeyboardKeyMode> on_key;

          public:
            KeyboardListener() noexcept;
            ~KeyboardListener() override;

            const ObjectType* object_type() const noexcept override;

          protected:
            bool handle_event(IEvent* e) noexcept override;
        };

        class FocusListener : public SingleChildWidget {
          public:
            Signal<bool> on_focus;

          public:
            FocusListener() noexcept;

            void focus() noexcept;
            void unfocus() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            bool handle_event(IEvent* e) noexcept override;
        };

        class CharInputListener : public SingleChildWidget {
          public:
            Signal<uint32_t> on_input;

            CharInputListener() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets

    RolUI_decl_object_type_of(widgets::PointerListener);
    RolUI_decl_object_type_of(widgets::MouseListener);
    RolUI_decl_object_type_of(widgets::KeyboardListener);
    RolUI_decl_object_type_of(widgets::FocusListener);
    RolUI_decl_object_type_of(widgets::CharInputListener);

} // namespace RolUI
