#pragma once

#include <stdint.h>

#include "Point.h"
#include "Size.h"

namespace RolUI {

    struct Rect {
        int32_t x, y;
        uint32_t width, height;

        Rect() noexcept : x(0), y(0), width(0), height(0) {}
        Rect(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
            : x(x), y(y), width(w), height(h) {}

        Point pos() const noexcept { return Point(x, y); }
        Size size() const noexcept { return Size(width, height); }

        inline bool is_contain_point(int32_t x, int32_t y) const noexcept {
            if (x < this->x || y < this->y) return false;
            int32_t rbx = this->x + this->width - 1;
            int32_t rby = this->y + this->height - 1;
            if (x > rbx || y > rby) return false;
        }
    };

} // namespace RolUI
