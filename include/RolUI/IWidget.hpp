#pragma once

namespace RolUI {

    class IPainter;
    class Style;

    class IWidget {
      public:
        virtual ~IWidget() {}

        virtual void set_style(const Style& style) {}
        virtual void on_draw(IPainter* painter) {}
    };

} // namespace RolUI
