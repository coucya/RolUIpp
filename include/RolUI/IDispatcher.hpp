#pragma once

namespace RolUI {

    class IDispatcher {

      public:
        virtual ~IDispatcher() {}

        virtual void dispatch_event() = 0;

    };

} // namespace RolUI
