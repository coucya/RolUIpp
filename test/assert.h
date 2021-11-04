#pragma once

#include <cstdio>
#include <chrono>
#include <exception>

namespace RolUI {

    inline void _assert_print(const char* msg) { printf("%s", msg); }
    inline void _assert_print_fl(const char* file, size_t line) {
        printf("file: %s:%llu", file, line);
    }
    inline void _assert_print_error(const char* msg, const char* file, size_t line) {
        printf("    ");
        _assert_print_fl(file, line);
        printf("\n    what: %s \n\n", msg);
    }

    class AssertError : public std::exception {
      public:
        size_t line;
        const char* file;
        const char* msg;

        AssertError(const char* msg = "", const char* file = "", size_t line = 0)
            : msg(msg), file(file), line(line) {}

        const char* what() const noexcept override { return msg; }
    };

} // namespace RolUI

#define debug_assert(exp)                                       \
    do {                                                        \
        if ((exp) != true)                                      \
            throw RolUI::AssertError(#exp, __FILE__, __LINE__); \
    } while (false)

#define debug_assert_binary(exp1, exp2, op)                                        \
    do {                                                                           \
        if (!((exp1)op(exp2)))                                                     \
            throw RolUI::AssertError(#exp1 " " #op " " #exp2, __FILE__, __LINE__); \
    } while (false)

#define debug_assert_eq(exp1, exp2) debug_assert_binary(exp1, exp2, ==)
#define debug_assert_ne(exp1, exp2) debug_assert_binary(exp1, exp2, !=)
#define debug_assert_lt(exp1, exp2) debug_assert_binary(exp1, exp2, <)
#define debug_assert_gt(exp1, exp2) debug_assert_binary(exp1, exp2, >)
#define debug_assert_le(exp1, exp2) debug_assert_binary(exp1, exp2, <=)
#define debug_assert_ge(exp1, exp2) debug_assert_binary(exp1, exp2, ?=)

#define __assert_print_test_error__(func, msg, file, line)    \
    do {                                                      \
        RolUI::_assert_print("\n");                           \
        RolUI::_assert_print("\rtest " #func " failure: \n"); \
        RolUI::_assert_print_error(msg, file, line);          \
    } while (false)

#define test_func(func, args...)                                             \
    do {                                                                     \
        try {                                                                \
            func(args);                                                      \
            RolUI::_assert_print("\rtest " #func " done.\n");                \
        } catch (const RolUI::AssertError& e) {                               \
            __assert_print_test_error__(func, e.what(), e.file, e.line);     \
        } catch (const std::exception& e) {                                  \
            __assert_print_test_error__(func, e.what(), __FILE__, __LINE__); \
        }                                                                    \
    } while (false);

#define test_func_time(func, args...)                                        \
    do {                                                                     \
        try {                                                                \
            auto start = std::chrono::high_resolution_clock::now();          \
            func(args);                                                      \
            auto end = std::chrono::high_resolution_clock::now();            \
            auto d = std::chrono::duration<double, std::milli>(end - start); \
            printf("\rtest " #func " done. time: %.2f ms\n", d.count());     \
        } catch (const RolUI::AssertError& e) {                              \
            __assert_print_test_error__(func, e.what(), e.file, e.line);     \
        } catch (const std::exception& e) {                                  \
            __assert_print_test_error__(func, e.what(), __FILE__, __LINE__); \
        }                                                                    \
    } while (false);
