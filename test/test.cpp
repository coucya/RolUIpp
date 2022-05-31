
#include <corecrt.h>
#include <cstdio>

#include "assert.h"
// #include "RolUI/sigslot/Signal.hpp"
#include "RolUI/sigslot.hpp"
#include "RolUI/Object.hpp"

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

        debug_assert_eq(s.slot_count(), 3);

        debug_assert_eq(a, 10);
        s.emit(n);
        debug_assert_eq(a, 11);

        s.disconnect(h1);
        s.disconnect(h2);
        s.disconnect(h3);

        printf("========== end   %2d ==========\n", n);
    }
    debug_assert_eq(s.slot_count(), 0);

    {
        int n = 2;
        printf("\n\n========== start %2d ==========\n", n);

        A slot;

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.slot_count(), 1);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.slot_count(), 2);

        debug_assert_eq(slot.signals_count(), 2);

        s.emit(n);

        s.disconnect(h2);
        debug_assert_eq(s.slot_count(), 1);

        s.disconnect(h3);
        debug_assert_eq(s.slot_count(), 0);

        printf("========== end   %2d ==========\n", n);
    }

    {
        int n = 3;
        printf("\n\n========== start %2d ==========\n", n);

        A slot;

        size_t h1 = s.connect(&slot);
        debug_assert_eq(s.slot_count(), 1);

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.slot_count(), 2);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.slot_count(), 3);

        debug_assert_eq(slot.signals_count(), 3);

        s.emit(n);

        s.disconnect_all();

        printf("========== end   %2d ==========\n", n);
    }
    debug_assert_eq(s.slot_count(), 0);

    {
        int n = 4;
        printf("\n\n========== start %2d ==========\n", n);

        A slot;

        size_t h1 = s.connect(&slot);
        debug_assert_eq(s.slot_count(), 1);

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.slot_count(), 2);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.slot_count(), 3);

        size_t h4 = s.connect(foo);
        debug_assert_eq(s.slot_count(), 4);

        size_t h5 = s.connect(A::s_foo);
        debug_assert_eq(s.slot_count(), 5);

        debug_assert_eq(slot.signals_count(), 3);

        s.emit(n);

        s.disconnect_all();

        printf("========== end   %2d ==========\n", n);
    }
    debug_assert_eq(s.slot_count(), 0);

    {
        int n = 5;
        printf("\n\n========== start %d ==========\n", n);

        A slot;

        size_t h1 = s.connect(&slot);
        debug_assert_eq(s.slot_count(), 1);

        size_t h2 = s.connect(&slot, &A::foo);
        debug_assert_eq(s.slot_count(), 2);

        size_t h3 = s.connect(&slot, &A::bar);
        debug_assert_eq(s.slot_count(), 3);

        debug_assert_eq(slot.signals_count(), 3);

        s.emit(n);

        slot.disconnect_all();

        printf("========== end   %2d ==========\n", n);
    }
}

int main() {
    test_func(sigslot_test);

    return 0;
}
