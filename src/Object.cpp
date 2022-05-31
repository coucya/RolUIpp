#include "RolUI/Object.hpp"

namespace RolUI {
    const char* ObjectType::type_name() const noexcept { return _type_name; }

    bool ObjectType::is_superclass(const ObjectType* other) const noexcept {
        for (const ObjectType* it = this; it != nullptr; it = it->_parent)
            if (it == other)
                return true;
        return false;
    }
    bool ObjectType::is_superclass(const std::type_info& other) const noexcept {
        for (const ObjectType* it = this; it != nullptr; it = it->_parent)
            if (*it->_type_info == other)
                return true;
        return false;
    }

    Object::Object() noexcept {}
    Object::~Object() {}

    bool Object::object_type_is(const ObjectType* ot) const noexcept {
        return object_type()->is_superclass(ot);
    }

    Object* Object::object_ref() noexcept {
        _ref_count++;
        return this;
    }
    Object* Object::object_unref() noexcept {
        _ref_count = _ref_count > 0 ? _ref_count - 1 : _ref_count;
        return this;
    }

    template <>
    const ObjectType* object_type_of<Object>() noexcept {
        static const ObjectType ot{"Object", typeid(Object), nullptr};
        return &ot;
    }

} // namespace RolUI
