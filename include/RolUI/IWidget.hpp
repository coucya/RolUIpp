#pragma once

namespace RolUI {

    class IPainter;

    class IWidget {
      public:
        virtual ~IWidget() {}

        virtual void on_draw(IPainter* painter) {}
    };

} // namespace RolUI
