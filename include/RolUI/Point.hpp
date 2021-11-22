#pragma once

#include "RolUI/Vector.hpp"

namespace RolUI {

    struct Point : public Vector<int> {
        Point() noexcept : Vector<int>(0, 0) {}
        Point(int x, int y) noexcept : Vector<int>(x, y) {}
        Point(const Vector<int>& v) noexcept : Vector<int>(v) {}
    };

} // namespace RolUI
