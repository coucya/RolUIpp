
#include <corecrt.h>
#include <cstdio>

#include "assert.h"
#include "RolUI/sigsslot/Signal.hpp"

using namespace RolUI;

struct A : public HasSlot {
    void foo(int n) const noexcept {
        printf("A::foo(%d)\n", n);
    }
    void bar(int n) {
        printf("A::bar(%d)\n", n);
    }

    void operator()(int n) {
        printf("A::operator()(%d)\n", n);
    }

    static void s_foo(int n) { printf("s_foor(%d)\n", n); }
};
void foo(int n) { printf("foo(%d)\n", n); }

void sigslot_test() {
    Signal<int> s;

    {
        int n = 1;
        int a = 10;
        printf("\n\n========== start %2d ==========\n", n);

        size_t h1 = s.connect([&](int n) { printf("[&a](%d)\n", n); a += n; });
        size_t h2 = s.connect(&A::s_foo);
        size_t h3 = s.connect(foo);

        debug_assert_eq(s.size(), 3);

        debug_assert_eq(a, 10);
        s.emit(n);
        debug_assert_eq(a, 11);

        s.disconnect(h1);
        s.disconnect(h2);
        s.disconnect(h3);

        printf("========== end   %2d ==========\n", n);
    }
    debug_assert_eq(s.size(), 0);

    {
        int n = 2;
        printf("\n\n========== start %2d ==========\n", n);

        A slot;
        size_t h1 = s.connect(slot);
        debug_assert_eq(s.size(), 1);

        size_t h2 = s.connect(slot, &A::foo);
        debug_assert_eq(s.size(), 2);

        size_t h3 = s.connect(slot, &A::bar);
        debug_assert_eq(s.size(), 3);

        debug_assert_eq(slot.size(), 0);

        s.emit(n);

        s.disconnect(h1);
        debug_assert_eq(s.size(), 2);

        s.disconnect(h2);
        debug_assert_eq(s.size(), 1);

        s.disconnect(h3);
        debug_assert_eq(s.size(), 0);

        printf("========== end   %2d ==========\n", n);
    }

    {
        int n = 3;
        printf("\n\n========== start %2d ==========\n", n);

        A slot;

        size_t h1 = s.connect(&slot);
        debug_assert_eq(s.size(), 1);

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.size(), 2);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.size(), 3);

        debug_assert_eq(slot.size(), 3);

        s.emit(n);

        printf("========== end   %2d ==========\n", n);
    }
    debug_assert_eq(s.size(), 0);

    {
        int n = 4;
        printf("\n\n========== start %2d ==========\n", n);

        A slot;

        size_t h1 = s.connect(&slot);
        debug_assert_eq(s.size(), 1);

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.size(), 2);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.size(), 3);

        size_t h4 = s.connect(foo);
        debug_assert_eq(s.size(), 4);

        size_t h5 = s.connect(A::s_foo);
        debug_assert_eq(s.size(), 5);

        debug_assert_eq(slot.size(), 3);

        s.emit(n);

        s.disconnect(&slot);
        debug_assert_eq(s.size(), 4);
        printf("========== dis: &slot\n");
        s.emit(n);

        s.disconnect(&slot, &A::foo);
        debug_assert_eq(s.size(), 3);
        printf("== dis: &A::foo\n");
        s.emit(n);

        s.disconnect(&slot, &A::bar);
        debug_assert_eq(s.size(), 2);
        printf("== dis: &A::bar\n");
        s.emit(n);

        s.disconnect(foo);
        debug_assert_eq(s.size(), 1);
        printf("== dis: foo\n");
        s.emit(n);

        s.disconnect(A::s_foo);
        debug_assert_eq(s.size(), 0);
        printf("== dis: s_foo\n");
        s.emit(n);

        printf("========== end   %2d ==========\n", n);
    }
    debug_assert_eq(s.size(), 0);

    {
        int n = 5;
        printf("\n\n========== start %d ==========\n", n);

        A slot;

        size_t h1 = s.connect(&slot);
        debug_assert_eq(s.size(), 1);

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.size(), 2);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.size(), 3);

        debug_assert_eq(slot.size(), 3);

        s.emit(n);


        slot.disconnect(&s, &A::foo);
        debug_assert_eq(s.size(), 2);
        printf("== dis: &A::foo\n");
        s.emit(n);

        printf("========== end   %2d ==========\n", n);
    }
}

int main() {
    test_func(sigslot_test);
    return 0;
}
