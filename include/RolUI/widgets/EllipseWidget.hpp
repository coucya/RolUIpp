#pragma ocne

#include <stdint.h>

#include "RolUI/Color.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    class EllipseWidget : public Widget {

      private:
        uint32_t _border_width;
        Color _border_color;
        Color _background_color;

      public:
        EllipseWidget() noexcept : _border_width(0) {}
        EllipseWidget(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept;

        ~EllipseWidget() override;

        void set_border_width(uint32_t w) noexcept;

        void set_border_color(const Color& c) noexcept;
        void set_background_color(const Color& c) noexcept;

        void draw(IPainter* painter) override;
    };

} // namespace RolUI
