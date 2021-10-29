#pragma once

#include <stdint.h>

namespace RolUI {

    inline uint32_t strlen(const char* str) {
        uint32_t len = 0;
        const char* str_it = str;
        while (*(str_it++) != '\0') len++;
        return len;
    }

} // namespace RolUI
