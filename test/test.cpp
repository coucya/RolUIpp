
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

struct ObjA;
struct ObjB;
struct ObjC;

namespace RolUI {
    RolUI_decl_object_type_of(ObjA);
    RolUI_decl_object_type_of(ObjB);
    RolUI_decl_object_type_of(ObjC);
} // namespace RolUI

struct InterfaceA {
    virtual ~InterfaceA() {}
    virtual void foo() = 0;
};
struct ObjA : public RolUI::Object {
    const ObjectType* object_type() const noexcept override { return object_type_of<ObjA>(); }
};
struct ObjB : public InterfaceA, public RolUI::Object {
    void foo() override {}
    const ObjectType* object_type() const noexcept override { return object_type_of<ObjB>(); }
};
struct ObjC : public InterfaceA, public ObjA {
    void foo() override {}
    const ObjectType* object_type() const noexcept override { return object_type_of<ObjB>(); }
};

namespace RolUI {
    RolUI_impl_object_type_of(ObjA, RolUI::Object);
    RolUI_impl_object_type_of(ObjB, InterfaceA, RolUI::Object);
    RolUI_impl_object_type_of(ObjC, InterfaceA, ObjA);
} // namespace RolUI

void Object_test() {
    ObjA obj_a;
    ObjB obj_b;
    ObjC obj_c;
    ObjA* optr_a = &obj_a;
    ObjB* optr_b = &obj_b;
    ObjC* optr_c = &obj_c;
    InterfaceA* iptr_b = &obj_b;
    InterfaceA* iptr_c = &obj_c;

    const ObjectType* ot_a = object_type_of<ObjA>();
    const ObjectType* ot_b = object_type_of<ObjB>();
    const ObjectType* ot_c = object_type_of<ObjC>();

    debug_assert(ot_a->is_superclass<RolUI::Object>());
    debug_assert(ot_b->is_superclass<RolUI::Object>());
    debug_assert(ot_a->is_superclass(object_type_of<RolUI::Object>()));
    debug_assert(ot_b->is_superclass(object_type_of<RolUI::Object>()));
    debug_assert(ot_a->is_superclass(typeid(RolUI::Object)));
    debug_assert(ot_b->is_superclass(typeid(RolUI::Object)));
    debug_assert(ot_b->is_superclass(typeid(InterfaceA)));
    debug_assert(!ot_a->is_superclass<InterfaceA>());
    debug_assert(ot_b->is_superclass<InterfaceA>());
    debug_assert(ot_c->is_superclass<Object>());
    debug_assert(ot_c->is_superclass<ObjA>());
    debug_assert(ot_c->is_superclass<InterfaceA>());

    debug_assert(ot_a->try_cast<ObjA>(static_cast<Object*>(&obj_a)) == optr_a);
    debug_assert(ot_a->try_cast<ObjB>(static_cast<Object*>(&obj_a)) == nullptr);

    debug_assert(object_try_cast<ObjA>(static_cast<Object*>(&obj_a)) == optr_a);
    debug_assert(object_try_cast<ObjB>(static_cast<Object*>(&obj_a)) == nullptr);
    debug_assert(object_try_cast<InterfaceA>(static_cast<Object*>(&obj_a)) == nullptr);
    debug_assert(object_try_cast<InterfaceA>(static_cast<Object*>(&obj_b)) == iptr_b);
    debug_assert(object_try_cast<InterfaceA>(static_cast<Object*>(&obj_c)) == iptr_c);
}

int main() {
    test_func(sigslot_test);
    test_func(Object_test);

    return 0;
}
