#pragma once

#include "./Size.hpp"
#include "./Point.hpp"

namespace RolUI {

    class IPainter;
    // class Style;

    struct Constraint {
        Size min;
        Size max;
    };

    class IWidget {
      public:
        virtual ~IWidget() {}

      protected:
        virtual void on_draw(IPainter* painter) {}

        virtual Size perlayout(Constraint constraint) = 0;
        virtual bool hit_test(Point pos) = 0;
    };

} // namespace RolUI
