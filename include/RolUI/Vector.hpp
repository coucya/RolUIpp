#pragma once

namespace RolUI {

    struct Vector {
        union {
            int x, width;
        };
        union {
            int y, height;
        };

        Vector() noexcept : x(0), y(0) {}
        Vector(int x, int y) noexcept
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
