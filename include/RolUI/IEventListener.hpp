#pragma once

namespace RolUI {

    class IEvent;

    class IEventListener {
      public:
        virtual ~IEventListener() {}

        virtual bool handle_event(IEvent* event) = 0;
    };

} // namespace RolUI
