#pragma ocne

#include <stdint.h>

#include "RolUI/Color.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    class RectWidget : public Widget {
      private:
        uint32_t _round = 0;
        uint32_t _border_width = 0;
        Color _border_color;
        Color _background_color;

      public:
        RectWidget() noexcept {}
        RectWidget(int32_t x, int32_t y, uint32_t w, uint32_t h,
                   uint32_t round = 0) noexcept;

        ~RectWidget() override;

        void set_round(uint32_t r) noexcept;
        void set_border_width(uint32_t w) noexcept;

        void set_border_color(const Color& c) noexcept;
        void set_background_color(const Color& c) noexcept;

        void on_draw(IPainter* painter) override;
    };

} // namespace RolUI
