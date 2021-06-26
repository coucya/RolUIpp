#pragma once

#include <stdint.h>

namespace RolUI {

    struct Point {
        int32_t x, y;

        Point() noexcept : x(0), y(0) {}
        Point(int32_t x, int32_t y) noexcept
            : x(x), y(y) {}

        Point& operator+=(const Point& p) noexcept {
            x += p.x, y += p.y;
            return *this;
        }
        Point& operator-=(const Point& p) noexcept {
            x -= p.x, y -= p.y;
            return *this;
        }

        Point operator+(const Point& p) const noexcept {
            return {x + p.x, y + p.y};
        }
        Point operator-(const Point& p) const noexcept {
            return {x - p.x, y - p.y};
        }
    };

} // namespace RolUI
