#pragma once

#include "RolUI/IEvent.hpp"
#include "RolUI/IWidget.hpp"
#include <stdint.h>
#include <string_view>

#define RolUI_define_event_type_in_class(tp) \
    static const EventType* type() {         \
        static EventType et{#tp};            \
        return &et;                          \
    }

#define RolUI_decl_event_type_in_class(tp) \
    static const EventType* type();

#define RolUI_impl_event_type_in_class(tp) \
    const EventType* tp::type() {          \
        static EventType et{#tp};          \
        return &et;                        \
    }

#define RolUI_define_event_type(tp)       \
    inline const EventType* tp##_type() { \
        static EventType et{#tp};         \
        return &et;                       \
    }

#define RolUI_decl_event_type(tp) \
    const EventType* tp##_type();

#define RolUI_impl_event_type(tp)  \
    const EventType* tp##_type() { \
        static EventType et{#tp};  \
        return &et;                \
    }

namespace RolUI {

    class Widget;
    class IEvent;

    bool send_event(Widget* w, IEvent* e);

    class EventType {
      public:
        EventType(const char* name) noexcept : _name(name) {}
        EventType(std::string_view name) noexcept : _name(name) {}

        EventType(const EventType&) = delete;
        EventType(EventType&&) = delete;

        ~EventType() = default;

        EventType& operator=(const EventType& et) = delete;
        EventType& operator=(EventType&&) = delete;

        std::string_view name() const noexcept { return _name; }

      private:
        std::string_view _name;
    };

    class IEvent {

        friend bool send_event(Widget*, IEvent*);

      public:
        IEvent(const EventType* et, Widget* target) noexcept
            : _type(et), _target(target) {}

        virtual ~IEvent() {}

        const EventType* event_type() const noexcept { return _type; };

        bool is(const EventType* et) const noexcept { return event_type() == et; }

        Widget* target() const noexcept { return _target; }

      private:
        const EventType* _type;
        Widget* _target;
    };

} // namespace RolUI
