#pragma ocne

#include <stdint.h>

#include "RolUI/Color.h"
#include "RolUI/Rect.h"
#include "RolUI/Widget.h"
#include "RolUI/IPainter.h"

namespace RolUI {
    class EllipseWidget : public Widget {
      private:
        uint32_t _border_width;
        Color _border_color;
        Color _background_color;
        Rect _rect;

      public:
        EllipseWidget() noexcept : _border_width(0) {}
        EllipseWidget(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
            : _rect(x, y, w, h), _border_width(0) {}

        ~EllipseWidget() override;

        EllipseWidget& set_x(int32_t x) noexcept;
        EllipseWidget& set_y(int32_t y) noexcept;
        EllipseWidget& set_pos(int32_t x, int32_t y) noexcept;

        EllipseWidget& set_width(uint32_t w) noexcept;
        EllipseWidget& set_heigth(uint32_t h) noexcept;
        EllipseWidget& set_size(uint32_t w, uint32_t h) noexcept;

        EllipseWidget& set_border_width(uint32_t w) noexcept;

        EllipseWidget& set_border_color(const Color& c) noexcept;
        EllipseWidget& set_background_color(const Color& c) noexcept;

        void draw(IPainter* painter) override;
    };

} // namespace RolUI
