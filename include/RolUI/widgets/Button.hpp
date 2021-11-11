#pragma once

#include "RolUI/Color.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/sigslot/Slot.hpp"

#include "./Label.hpp"
#include "RolUI/widgets/Label.hpp"

namespace RolUI {
    namespace widget {

        class Button : public Label {
          public:
            typedef Label::StyleProperty StyleProperty;

            enum State {
                default_,
                hover,
                press,
            };

          public:
            Button() noexcept;
            Button(std::string text) noexcept;
            Button(std::string text, Widget* parent) noexcept;
            ~Button();

            const StyleProperty& default_property() const noexcept;
            const StyleProperty& hover_property() const noexcept;
            const StyleProperty& press_property() const noexcept;

            void set_default_property(const StyleProperty& property) noexcept;
            void set_hover_property(const StyleProperty& property) noexcept;
            void set_press_property(const StyleProperty& property) noexcept;

          public:
            Signal<> on_click;
            Signal<State> on_state_change;

          private:
            void _init() noexcept;
            void _init_event_bind() noexcept;

          private:
            bool _is_press = false;
            State _state = State::default_;

            StyleProperty _default_propertys;
            StyleProperty _hover_propertys;
            StyleProperty _press_propertys;
        };

    } // namespace widget
} // namespace RolUI
