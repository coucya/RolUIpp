#pragma once

#include "stdint.h"

namespace RolUI {

    struct Color {
        union {
            uint8_t r, g, b, a;
            uint8_t rgba[4];
        };
    };

} // namespace RolUI
