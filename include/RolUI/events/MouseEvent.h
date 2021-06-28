#pragma once

#include <stdint.h>

#include "RolUI/Point.h"
#include "RolUI/vector.h"
#include "RolUI/IEvent.h"

namespace RolUI {

    class MouseEvent : public IEvent {
      public:
        enum MouseKey {
            MouseKey_1 = 1,
            MouseKey_2,
            MouseKey_3,
            MouseKey_4,
            MouseKey_5,
            MouseKey_6,
            MouseKey_7,
            MouseKey_8,

            MouseKey_Left = MouseKey_1,
            MouseKey_right = MouseKey_2,
            MouseKey_middle = MouseKey_3,
        };

      public:
        MouseEvent(Point last_pos, Point current_pos) noexcept;
        ~MouseEvent() override;

        Point pos() const noexcept;
        Vector offset() const noexcept;
        MouseKey button() const noexcept;

        void set_widget_pos(Point p);

      private:
        Point _last_pos;
        Point _current_pos;
        Point _widget_pos;
    };

} // namespace RolUI
