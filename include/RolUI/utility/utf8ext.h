#pragma once

#include "./utf8.h"

#ifdef __cplusplus
extern "C" {
#endif

inline unsigned utf8utf8index(const utf8_int8_t* utf8_str, unsigned codepoint_idx) {
    const char* str_beg = utf8_str;
    const char* str_it = utf8_str;
    int cp;
    for (unsigned i = 0; i < codepoint_idx && *str_it != '\0'; i++)
        str_it = utf8codepoint(str_it, &cp);
    return str_it - str_beg;
}

inline unsigned utf8nutf8index(const utf8_int8_t* utf8_str, unsigned codepoint_idx, unsigned n) {
    const char* str_beg = utf8_str;
    const char* str_it = utf8_str;
    int cp;
    for (unsigned i = 0; i < codepoint_idx && *str_it != '\0' && str_it - str_beg < n; i++)
        str_it = utf8codepoint(str_it, &cp);
    if (str_it - str_beg > n)
        str_it = utf8rcodepoint(str_it, &cp);
    return str_it - str_beg;
}

inline unsigned utf8codepointindex(const utf8_int8_t* utf8_str, unsigned utf8_idx) {
    return utf8nlen(utf8_str, utf8_idx);
}

#ifdef __cplusplus
}
#endif
