#pragma once

#include <stdint.h>

#include "Rect.h"
#include "Size.h"
#include "Point.h"
#include "IPainter.h"

namespace RolUI {

    class IWidget {
      public:
        virtual ~IWidget() {}

        virtual Point pos() = 0;
        virtual Size size() = 0;
        virtual Rect rect() { return Rect{pos(), size()}; }

        virtual void draw(IPainter* painter) = 0;
    };

} // namespace RolUI
