#pragma once

#include <stdint.h>

#include "IEvent.h"

namespace RolUI {

    class IEventListener {
      public:
        bool do_event(IEvent* event) {
            return filter_event(event) ? on_event(event) : false;
        }
        virtual bool filter_event(const IEvent* event) { return true; }
        virtual bool on_event(IEvent* event) { return false; }
    };

} // namespace RolUI
