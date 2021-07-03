#pragma once

#include <stdint.h>
#include <typeinfo>
#include <type_traits>

namespace RolUI {

    class IEvent;

    class EventType {
      public:
        template <typename T>
        static EventType of() {
            static_assert(std::is_base_of<IEvent, T>::value);
            return EventType(typeid(T), typeid(T).name());
        }

      public:
        EventType(const std::type_info& ti, const char* name = "") noexcept
            : _type_info(ti), _name(name) {}

        bool operator==(const EventType& et) const noexcept {
            return _type_info == et._type_info;
        }
        bool operator!=(const EventType& et) const noexcept {
            return _type_info != et._type_info;
        }

        const char* name() const noexcept { return _name; }

      private:
        const char* _name;
        const std::type_info& _type_info;
    };

    class IEvent {
      public:
        virtual ~IEvent() {}

        virtual EventType event_type() const = 0;
    };

} // namespace RolUI

#define event_type_of(tp) EventType(typeid(tp), #tp)

#define impl_event_type_in_class(tp)        \
    EventType event_type() const override { \
        return event_type_of(tp);           \
    }

#define impl_event_type(tp)            \
    EventType tp::event_type() const { \
        return event_type_of(tp);      \
    }
