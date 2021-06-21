#pragma once

#include <stdint.h>

#include "IPainter.h"
namespace RolUI {

    class IWidget {
      public:
        virtual ~IWidget() {}

        virtual void draw(IPainter* painter) = 0;
    };

} // namespace RolUI
