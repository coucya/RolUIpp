#pragma once

#include "./Vector.hpp"

namespace RolUI {

    struct Size : public Vector<int> {
        Size() noexcept : Vector<int>(0, 0) {}
        Size(int x, int y) noexcept : Vector<int>(x, y) {}
        Size(const Vector<int>& v) noexcept : Vector<int>(v) {}
    };

} // namespace RolUI
