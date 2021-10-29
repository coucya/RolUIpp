#pragma once

#include <stdint.h>
#include <typeinfo>
#include <type_traits>

#define event_type_of(tp) (RolUI::EventType::of<tp>())

#define impl_event_type_in_class(tp)        \
    EventType event_type() const override { \
        return event_type_of(tp);           \
    }

#define impl_event_type(tp)            \
    EventType tp::event_type() const { \
        return event_type_of(tp);      \
    }

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
        EventType() noexcept : _type_info(nullptr), _name("") {}
        EventType(const std::type_info& ti, const char* name = "") noexcept
            : _type_info(&ti), _name(name) {}

        EventType(const EventType&) = default;
        EventType(EventType&&) = default;

        ~EventType() {}

        EventType& operator=(const EventType& et) = default;
        EventType& operator=(EventType&&) = default;

        bool operator==(const EventType& et) const noexcept {
            if (_type_info == nullptr && et._type_info == nullptr)
                return true;
            else if (_type_info != nullptr && et._type_info != nullptr)
                return *_type_info == *et._type_info;
            else
                return false;
        }
        bool operator!=(const EventType& et) const noexcept {
            if (_type_info == nullptr && et._type_info == nullptr)
                return false;
            else if (_type_info != nullptr && et._type_info != nullptr)
                return *_type_info != *et._type_info;
            else
                return true;
        }

        const char* name() const noexcept { return _name; }

      private:
        const char* _name;
        const std::type_info* _type_info;
    };

    class IEvent {
      public:
        virtual ~IEvent() {}

        virtual EventType event_type() const = 0;

        bool is(const EventType& et) const noexcept { return event_type() == et; }

        template <typename T>
        bool is() const noexcept {
            static_assert(std::is_base_of<IEvent, T>::value);
            return is(event_type_of(T));
        }

        template <typename T>
        T* cast() {
            static_assert(std::is_base_of<IEvent, T>::value);
            if (!is<T>()) return nullptr;
            return static_cast<T*>(this);
        }

        template <typename T>
        const T* cast() const {
            static_assert(std::is_base_of<IEvent, T>::value);
            if (!is<T>()) return nullptr;
            return static_cast<T*>(this);
        }
    };

} // namespace RolUI
