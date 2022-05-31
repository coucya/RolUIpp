#pragma once

#include <typeinfo>
#include <type_traits>

namespace RolUI {

    class Object;
    class ObjectType;

    class ObjectType {
      public:
        ObjectType(const char* type_name, const std::type_info& tp_info) noexcept
            : _type_name(type_name), _type_info(&tp_info) {}
        ObjectType(const char* type_name, const std::type_info& tp_info,
                   const ObjectType* parent) noexcept
            : _type_name(type_name), _type_info(&tp_info), _parent(parent) {}

        ~ObjectType() {}

        ObjectType(const ObjectType&) = delete;
        ObjectType(ObjectType&&) = delete;
        ObjectType& operator=(const ObjectType&) = delete;
        ObjectType& operator=(ObjectType&&) = delete;

        const char* type_name() const noexcept;

        bool is_superclass(const ObjectType* other) const noexcept;
        bool is_superclass(const std::type_info& other) const noexcept;

        template <typename T>
        bool is_superclass() const noexcept { return is_superclass(typeid(T)); }

        template <typename T>
        T* try_cast(Object* obj) const noexcept {
            return typeid(T) == *_type_info ? static_cast<T*>(obj) : nullptr;
        }
        template <typename T>
        const T* try_cast(const Object* obj) const noexcept {
            return typeid(T) == *_type_info ? static_cast<const T*>(obj) : nullptr;
        }

        template <typename T>
        T* try_as_superclass(Object* obj) const noexcept {
            return dynamic_cast<T*>(obj);
        }
        template <typename T>
        const T* try_as_superclass(const Object* obj) const noexcept {
            return dynamic_cast<const T*>(obj);
        }

      private:
        const char* _type_name = "";
        const std::type_info* _type_info = nullptr;
        const ObjectType* _parent = nullptr;
    };

    class Object {
      public:
        virtual ~Object();

        virtual const ObjectType* object_type() const noexcept = 0;

        template <typename T>
        bool object_type_is() const noexcept { return object_type()->is_superclass<T>(); };
        bool object_type_is(const ObjectType* ot) const noexcept;

        Object* object_ref() noexcept;
        Object* object_unref() noexcept;

      protected:
        Object() noexcept;

      private:
        unsigned _ref_count = 0;
    };

#define ___CONCAT_IMPL(l, r) l##r
#define ___CONCAT(l, r) ___CONCAT_IMPL(l, r)

#define ARG_COUNT(...) _INTERNAL_ARG_COUNT(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _INTERNAL_ARG_COUNT(_0, _1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

#define ___PCI_0() nullptr
#define ___PCI_1(T) object_type_of<T>()

#define RolUI_OBJECT_TYPE_OF(...)              \
    ___CONCAT(___PCI_, ARG_COUNT(__VA_ARGS__)) \
    (__VA_ARGS__)

#define RolUI_decl_object_type_of(T) \
    template <>                      \
    const ObjectType* object_type_of<T>() noexcept

#define RolUI_impl_object_type_of_with_namespace(NAMESPACE, T, ...)    \
    template <>                                                        \
    const ObjectType* object_type_of<NAMESPACE::T>() noexcept {        \
        static const ObjectType ot{#T, typeid(NAMESPACE::T),           \
                                   RolUI_OBJECT_TYPE_OF(__VA_ARGS__)}; \
        return &ot;                                                    \
    }

#define RolUI_impl_object_type_of(T, ...)                              \
    template <>                                                        \
    const ObjectType* object_type_of<T>() noexcept {                   \
        static const ObjectType ot{#T, typeid(T),                      \
                                   RolUI_OBJECT_TYPE_OF(__VA_ARGS__)}; \
        return &ot;                                                    \
    }

    template <typename T>
    const ObjectType* object_type_of() noexcept { return nullptr; }

    template <>
    const ObjectType* object_type_of<Object>() noexcept;

    template <typename T>
    T* object_try_cast(Object* obj) {
        if (!obj) return nullptr;
        return dynamic_cast<T*>(obj);
    }

} // namespace RolUI
