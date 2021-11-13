#pragma once

namespace RolUI {

    class IDispatcher {

      public:
        virtual ~IDispatcher() {}

        virtual void dispatch_event(double timeout) = 0;

    };

} // namespace RolUI
