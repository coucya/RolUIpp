#pragma once

#include <type_traits>
namespace RolUI {

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    struct Vector {
        union {
            T x, width;
        };
        union {
            T y, height;
        };

        Vector() noexcept : x(0), y(0) {}
        Vector(T x, T y) noexcept : x(x), y(y) {}

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

        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Vector& operator*=(const N& v) noexcept {
            x *= v, y *= v;
            return *this;
        }
        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Vector& operator/=(const N& v) noexcept {
            x /= v, y /= v;
            return *this;
        }

        Vector operator+(const Vector& v) const noexcept {
            return {x + v.x, y + v.y};
        }
        Vector operator-(const Vector& v) const noexcept {
            return {x - v.x, y - v.y};
        }

        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Vector operator*(const N& v) const noexcept {
            return {x * v, y * v};
        }
        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Vector operator/(const N& v) const noexcept {
            return {x / v, y / v};
        }
    };

    typedef Vector<int> Vec2i;
    typedef Vector<float> Vec2f;

} // namespace RolUI
