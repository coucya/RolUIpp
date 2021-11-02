#pragma once

namespace RolUI {

    class IPainter;

    class IDisplay {
      public:
        virtual ~IDisplay() {}

        virtual IPainter* painter() = 0;

        virtual void begin_draw() = 0;
        virtual void end_draw() = 0;

      private:
    };

} // namespace RolUI
