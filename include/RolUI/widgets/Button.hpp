#pragma once

#include "../Color.hpp"
#include "../IEvent.hpp"
#include "../Widget.hpp"
#include "../sigslot/Slot.hpp"

namespace RolUI {
    namespace widget {

        class Button : public Label {
          public:
            Button(Widget* parent = nullptr) noexcept;
            Button(Widget* parent, std::string text) noexcept;
            ~Button();

          public:
            Signal<> on_click;

          private:
            void _init() noexcept;
            void _init_event_bind() noexcept;

          private:
            bool _is_press = false;
        };

    } // namespace widget
} // namespace RolUI
