#pragma once

#include "./Object.hpp"

namespace RolUI {

    class Widget;
    class IEvent;

    bool send_event(Widget* w, IEvent* e);

    class IEvent : public Object {
        friend bool send_event(Widget*, IEvent*);

      public:
        IEvent(Widget* target) noexcept;
        ~IEvent() override;

        const ObjectType* object_type() const noexcept override;

        Widget* target() const noexcept;

      private:
        Widget* _target;
    };

    RolUI_decl_object_type_of(IEvent);

} // namespace RolUI
