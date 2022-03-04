#pragma once

#include "../Color.hpp"
#include "../sigslot/Signal.hpp"
#include "../sigslot/Slot.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class PointerListenerWidget : public SingleChildWidget {
          public:
            Signal<Point> on_up;
            Signal<Point> on_down;
            Signal<Point> on_click;
            Signal<Vec2i> on_move;
            Signal<Vec2i> on_drag;
            Signal<Vec2i> on_scroll;
            Signal<bool> on_hover;

          public:
            PointerListenerWidget() noexcept;
            ~PointerListenerWidget() override;

            virtual bool handle_event(IEvent* e) noexcept override;

            void draw(IPainter* painter) noexcept override;

          private:
            bool _is_press = false;
        };

    } // namespace widgets
} // namespace RolUI