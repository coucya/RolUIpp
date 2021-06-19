#include <stdint.h>

namespace RolUI {

    struct Point {
        int32_t x, y;

        Point() noexcept : x(0), y(0) {}
        Point(int32_t x, int32_t y) noexcept
            : x(x), y(y) {}
    };

} // namespace RolUI
