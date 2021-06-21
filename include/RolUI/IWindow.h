#pragma once

#include <stdint.h>

#include "IPainter.h"

namespace RolUI {

    class IWindow {
      public:
        virtual ~IWindow() {}
        virtual IPainter* painter() = 0;
        virtual void draw() = 0;
    };

} // namespace RolUI
