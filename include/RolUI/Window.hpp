#pragma once

#include <stdint.h>

#include "./IDisplay.hpp"
#include "./IDispatcher.hpp"
#include "./IEvent.hpp"
#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./sigslot/Slot.hpp"

namespace RolUI {

    class Application;
    class Widget;

    class Window : public IDisplay, public IDispatcher, public HasSlot {
        friend Application;

      public:
        Window() noexcept;
        ~Window() override;

        IPainter* painter() override;
        void begin_draw() override;
        void end_draw() override;

        void dispatch_event(double timeout) override;
    };

} // namespace RolUI
