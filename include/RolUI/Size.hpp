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

        Size operator+(const Size& v) const noexcept {
            return {width + v.width, height + v.height};
        }
        Size operator-(const Size& v) const noexcept {
            return {width - v.width, height - v.height};
        }

        Size operator*(float v) const noexcept {
            return {int(float(width) * v), int(float(height) * v)};
        }
        Size operator*(double v) const noexcept {
            return {int(double(width) * v), int(double(height) * v)};
        }
        Size operator*(int v) const noexcept { return {width * v, height * v}; }

        Size operator/(float v) const noexcept {
            return {int(float(width) / v), int(float(height) / v)};
        }
        Size operator/(double v) const noexcept {
            return {int(double(width) / v), int(double(height) / v)};
        }
        Size operator/(int v) const noexcept { return {width / v, height / v}; }
    };

} // namespace RolUI
