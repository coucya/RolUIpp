#pragma once

#include <typeinfo>
#include <type_traits>

namespace RolUI {

    class Object;
    class ObjectType;

    typedef const void* (*CastFunc)(const void*);

    class ParentClassInfo {
      public:
        ParentClassInfo() noexcept;
        ParentClassInfo(const std::type_info& info, const ObjectType* obj_tp = nullptr,
                        const CastFunc cast_func = nullptr) noexcept
            : _type_info(&info), _obj_type(obj_tp), _cast_func(cast_func) {}

        const std::type_info& type_info() const noexcept { return *_type_info; }
        const ObjectType* object_type() const noexcept { return _obj_type; }
        bool is_object_type() const noexcept { return _obj_type != nullptr; }
        const void* cast(const void* p) const noexcept { return _cast_func ? _cast_func(p) : p; }

      private:
        const std::type_info* _type_info = nullptr;
        const ObjectType* _obj_type = nullptr;
        const CastFunc _cast_func = nullptr;
    };

    class ObjectType {
      public:
        ObjectType(const char* type_name, const std::type_info& tp_info, CastFunc cast_func) noexcept
            : _tp_name(type_name), _tp_info(&tp_info), _cast_func(cast_func) {}
        ObjectType(const char* type_name, const std::type_info& tp_info, CastFunc cast_func,
                   unsigned parent_count, const ParentClassInfo* parent_infos) noexcept
            : _tp_name(type_name), _tp_info(&tp_info), _cast_func(cast_func),
              _parent_count(parent_infos ? parent_count : 0), _parent_infos(parent_infos) {}
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
            return typeid(T) == *_tp_info ? static_cast<T*>(obj) : nullptr;
        }
        template <typename T>
        const T* try_cast(const Object* obj) const noexcept {
            return typeid(T) == *_tp_info ? static_cast<const T*>(obj) : nullptr;
        }

        template <typename T>
        T* try_as_superclass(Object* obj) const noexcept {
            const void* ptr = _cast_func ? _cast_func(obj) : reinterpret_cast<const void*>(obj);
            const void* result_ptr = _try_as_superclass(ptr, typeid(T));
            return reinterpret_cast<T*>(const_cast<void*>(result_ptr));
        }
        template <typename T>
        const T* try_as_superclass(const Object* obj) const noexcept {
            const void* ptr = _cast_func ? _cast_func(obj) : reinterpret_cast<const void*>(obj);
            const void* result_ptr = _try_as_superclass(ptr, typeid(T));
            return reinterpret_cast<const T*>(result_ptr);
        }

      private:
        const void* _try_as_superclass(const void* obj, const std::type_info& tp_info) const noexcept;

      private:
        const char* _tp_name = "";
        const std::type_info* _tp_info = nullptr;
        unsigned _parent_count = 0;
        const ParentClassInfo* _parent_infos = nullptr;
        const CastFunc _cast_func = nullptr;
    };

    class Object {
      public:
        virtual ~Object();

        virtual const ObjectType* object_type() const noexcept = 0;

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

#define ___PCI(S, T) \
    ParentClassInfo { typeid(T), object_type_of<T>(), RolUI_cast_lambda(S, T) }
#define ___PCI_1(S, A) ___PCI(S, A)
#define ___PCI_2(S, A, B) ___PCI(S, A), ___PCI(S, B)
#define ___PCI_3(S, A, B, C) ___PCI(S, A), ___PCI(S, B), ___PCI(S, C)
#define ___PCI_4(S, A, B, C, D) ___PCI(S, A), ___PCI(S, B), ___PCI(S, C), ___PCI(S, D)
#define ___PCI_5(S, A, B, C, D, E) ___PCI(S, A), ___PCI(S, B), ___PCI(S, C), ___PCI(S, D), ___PCI(S, E)
#define ___PCI_6(S, A, B, C, D, E, f) ___PCI(S, A), ___PCI(S, B), ___PCI(S, C), ___PCI(S, D), ___PCI(S, E), ___PCI(S, F)
#define ___PCI_7(S, A, B, C, D, E, f, G) ___PCI(S, A), ___PCI(S, B), ___PCI(S, C), ___PCI(S, D), ___PCI(S, E), ___PCI(S, F), ___PCI(S, G)
#define ___PCI_8(S, A, B, C, D, E, f, G, H) ___PCI(S, A), ___PCI(S, B), ___PCI(S, C), ___PCI(S, D), ___PCI(S, E), ___PCI(S, F), ___PCI(S, G), ___PCI(S, H)
#define RolUI_ParentClassInfo_list(SelfType, ...) \
    ___CONCAT(___PCI_, ARG_COUNT(__VA_ARGS__))    \
    (SelfType, __VA_ARGS__)

#define RolUI_cast_lambda(FromType, ToType) [](const void* p) -> const void* { \
    return static_cast<const ToType*>(reinterpret_cast<const FromType*>(p));   \
}
#define RolUI_decl_object_type_of(T) \
    template <>                      \
    const ObjectType* object_type_of<T>() noexcept;

#define RolUI_impl_object_type_of(T, ...)                                                         \
    template <>                                                                                   \
    const ObjectType* object_type_of<T>() noexcept {                                              \
        static ParentClassInfo parent_infos[]{RolUI_ParentClassInfo_list(T, __VA_ARGS__)};        \
        static const ObjectType ot{#T, typeid(T), RolUI_cast_lambda(Object, T),                   \
                                   sizeof(parent_infos) / sizeof(ParentClassInfo), parent_infos}; \
        return &ot;                                                                               \
    }

    template <typename T>
    const ObjectType* object_type_of() noexcept { return nullptr; }

    template <>
    const ObjectType* object_type_of<Object>() noexcept;

    template <typename T>
    T* object_try_cast(Object* obj) {
        if (!obj) return nullptr;

        if (obj->object_type()->is_superclass<T>())
            return obj->object_type()->try_as_superclass<T>(obj);

        if constexpr (std::is_base_of<Object, T>::value) {
            if (object_type_of<T>() && object_type_of<T>()->is_superclass(obj->object_type()))
                return static_cast<T*>(obj);
        }
        return nullptr;
    }

} // namespace RolUI
