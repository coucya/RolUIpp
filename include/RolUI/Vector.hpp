#pragma once

#include <stdint.h>

namespace RolUI {

    struct Vector {
        int32_t x, y;

        Vector() noexcept : x(0), y(0) {}
        Vector(int32_t x, int32_t y) noexcept
            : x(x), y(y) {}

        bool operator==(const Vector& v) noexcept {
            return x == v.x && y == v.y;
        }
        bool operator!=(const Vector& v) noexcept {
            return x != v.x || y != v.y;
        }

        Vector& operator+=(const Vector& v) noexcept {
            x += v.x, y += v.y;
            return *this;
        }
        Vector& operator-=(const Vector& v) noexcept {
            x -= v.x, y -= v.y;
            return *this;
        }

        Vector operator+(const Vector& v) const noexcept {
            return {x + v.x, y + v.y};
        }
        Vector operator-(const Vector& v) const noexcept {
            return {x - v.x, y - v.y};
        }
    };

} // namespace RolUI
