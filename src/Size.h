#include <stdint.h>

namespace RolUI {

    struct Size {
        uint32_t width, height;

        Size() noexcept : width(0), height(0) {}
        Size(uint32_t w, uint32_t h) noexcept : width(w), height(h) {}
    };

} // namespace RolUI
