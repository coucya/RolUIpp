#pragma once

#include "./Point.hpp"
#include "./Size.hpp"
#include "./sigslot/Signal.hpp"

namespace RolUI {

    class IPainter;

    class IDisplay {
      public:
        virtual ~IDisplay() {}

        virtual IPainter* painter() = 0;

        virtual void begin_draw() = 0;
        virtual void end_draw() = 0;

        virtual Point pos() const = 0;
        virtual Size size() const = 0;

      public:
        Signal<Point> on_pos_change;
        Signal<Size> on_size_change;
    };

} // namespace RolUI
