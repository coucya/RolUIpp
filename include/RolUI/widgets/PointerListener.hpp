#pragma once

#include "../Color.hpp"
#include "../Widget.hpp"
#include "../sigslot/Signal.hpp"
#include "../sigslot/Slot.hpp"

namespace RolUI {
    namespace widget {

        class PointerListener : public Widget {
          public:
            Signal<Point> on_up;
            Signal<Point> on_down;
            Signal<Point> on_click;
            Signal<Size> on_move;
            Signal<bool> on_hover;

          public:
            PointerListener() noexcept;
            ~PointerListener() override;

          protected:
            void on_draw(IPainter* painter) override;
            Size perlayout(Constraint constraint) override;

          private:
            void _init_event_bind() noexcept;
        };

    } // namespace widget
} // namespace RolUI