#pragma once

#include "stdint.h"

namespace RolUI {

    struct Color {
        union {
            struct {
                uint8_t r, g, b, a;
            };
            uint8_t rgba[4];
        };

        Color() noexcept : r(0), g(0), b(0), a(255) {}
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
            : r(r), g(g), b(b), a(a) {}

        bool operator==(const Color& c) const noexcept {
            return r == c.r && g == c.g && b == c.b && a == c.a;
        }
        bool operator!=(const Color& c) const noexcept {
            return r != c.r && g != c.g && b != c.b && a != c.a;
        }
    };

} // namespace RolUI
