#pragma once

#include "./Vector.hpp"

namespace RolUI {

    struct Size {
        int width, height;

        Size() noexcept : width(0), height(0) {}
        Size(int w, int h) noexcept : width(w), height(h) {}
        Size(const Vector<int>& v) noexcept : Size(v.x, v.y) {}

        bool operator==(const Size& v) const noexcept {
            return width == v.width && height == v.height;
        }
        bool operator!=(const Size& v) const noexcept {
            return width != v.width || height != v.height;
        }

        Size& operator+=(const Size& v) noexcept {
            width += v.width, height += v.height;
            return *this;
        }
        Size& operator-=(const Size& v) noexcept {
            width -= v.width, height -= v.height;
            return *this;
        }

        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Size& operator*=(const N& v) noexcept {
            width *= v, height *= v;
            return *this;
        }
        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Size& operator/=(const N& v) noexcept {
            width /= v, height /= v;
            return *this;
        }

        Size operator+(const Size& v) const noexcept {
            return {width + v.width, height + v.height};
        }
        Size operator-(const Size& v) const noexcept {
            return {width - v.width, height - v.height};
        }

        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Size operator*(const N& v) const noexcept {
            return {width * v, height * v};
        }
        template <typename N, typename = std::enable_if_t<std::is_arithmetic_v<N>>>
        Size operator/(const N& v) const noexcept {
            return {width / v, height / v};
        }
    };

} // namespace RolUI
