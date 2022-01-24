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
            Signal<Vec2i> on_move;
            Signal<bool> on_hover;

          public:
            PointerListener() noexcept;
            ~PointerListener() override;

            virtual bool handle_event(IEvent* e) override;

          private:
            bool _is_press = false;
        };

    } // namespace widget
} // namespace RolUI