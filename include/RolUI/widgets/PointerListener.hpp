#pragma once

#include "../Color.hpp"
#include "../sigslot/Signal.hpp"
#include "../sigslot/Slot.hpp"
#include "./container.hpp"

namespace RolUI {
    namespace widget {

        class PointerListener : public SingleChildWidget {
          public:
            Signal<Point> on_up;
            Signal<Point> on_down;
            Signal<Point> on_click;
            Signal<Size> on_move;
            Signal<bool> on_hover;

          public:
            PointerListener() noexcept;
            ~PointerListener() override;

          private:
            void _init_event_bind() noexcept;
        };

    } // namespace widget
} // namespace RolUI