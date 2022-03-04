#pragma once

#include <type_traits>
namespace RolUI {

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    struct Vector {
        T x, y;

        Vector() noexcept : x(0), y(0) {}
        Vector(T x, T y) noexcept : x(x), y(y) {}

        bool operator==(const Vector& v) const noexcept {
            return x == v.x && y == v.y;
        }
        bool operator!=(const Vector& v) const noexcept {
            return x != v.x || y != v.y;
        }

        Vector operator+(const Vector& v) const noexcept {
            return {x + v.x, y + v.y};
        }
        Vector operator-(const Vector& v) const noexcept {
            return {x - v.x, y - v.y};
        }

        Vector operator*(float v) const noexcept {
            return {T(float(x) * v), T(float(y) * v)};
        }
        Vector operator*(double v) const noexcept {
            return {T(double(x) * v), T(double(y) * v)};
        }
        Vector operator*(int v) const noexcept {
            return {T(x * v), T(y * v)};
        }

        Vector operator/(float v) const noexcept {
            return {T(float(x) / v), T(float(y) / v)};
        }
        Vector operator/(double v) const noexcept {
            return {T(double(x) / v), T(double(y) / v)};
        }
        Vector operator/(int v) const noexcept {
            return {T(x / v), T(y / v)};
        }
    };

    typedef Vector<int> Vec2i;
    typedef Vector<float> Vec2f;

} // namespace RolUI
