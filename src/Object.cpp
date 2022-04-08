#include "RolUI/Object.hpp"

namespace RolUI {
    const char* ObjectType::type_name() const noexcept { return _tp_name; }

    bool ObjectType::is_superclass(const ObjectType* other) const noexcept {
        if (this == other) return true;
        for (int i = 0; i < _parent_count; i++)
            if (_parent_infos[i].object_type() && _parent_infos[i].object_type()->is_superclass(other))
                return true;
        return false;
    }
    bool ObjectType::is_superclass(const std::type_info& other) const noexcept {
        if (*_tp_info == other) return true;
        for (int i = 0; i < _parent_count; i++) {
            if (_parent_infos[i].type_info() == other)
                return true;
            if (_parent_infos[i].is_object_type() && _parent_infos[i].object_type()->is_superclass(other))
                return true;
        }
        return false;
    }

    const void* ObjectType::_try_as_superclass(const void* obj, const std::type_info& tp_info) const noexcept {
        if (*_tp_info == tp_info)
            return obj;

        for (int i = 0; i < _parent_count; i++) {
            auto pinfo = _parent_infos[i];
            if (pinfo.is_object_type()) {
                return pinfo.object_type()->_try_as_superclass(pinfo.cast(obj), tp_info);
            } else if (pinfo.type_info() == tp_info) {
                return pinfo.cast(obj);
            }
        }
        return nullptr;
    }

    Object::Object() noexcept {}
    Object::~Object() {}

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
        static ParentClassInfo parent_infos[]{};
        static const ObjectType ot{"Object", typeid(Object), RolUI_cast_lambda(Object, Object),
                                   sizeof(parent_infos) / sizeof(ParentClassInfo), parent_infos};
        return &ot;
    }

} // namespace RolUI