#pragma once

#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"

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
    };

} // namespace RolUI
