#pragma once

#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./sigslot.hpp"

namespace RolUI {

    class Application;
    class Widget;
    class IPainter;

    class Window {
        friend Application;

      public:
        virtual ~Window() {}

        virtual Point pos() const = 0;
        virtual Size size() const = 0;

        virtual IPainter* painter() = 0;
        virtual void begin_draw() = 0;
        virtual void end_draw() = 0;

        virtual void dispatch_event() = 0;

      public:
        Signal<Point> on_pos_change;
        Signal<Size> on_size_change;
    };

} // namespace RolUI
