#pragma once

#include "RolUI/Color.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/sigslot/Slot.hpp"

#include "./Label.hpp"

namespace RolUI {
    namespace widget {

        class Button : public Label {
          public:
            Button() noexcept;
            Button(std::string text) noexcept;
            Button(std::string text, Widget* parent) noexcept;
            ~Button();

          public:
            Signal<> on_click;

          private:
            void _init() noexcept;
            void _init_event_bind() noexcept;

          private:
            bool _is_press;
        };

    } // namespace widget
} // namespace RolUI
