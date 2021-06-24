#pragma ocne

#include <stdint.h>

#include "RolUI/Color.h"
#include "RolUI/Rect.h"
#include "RolUI/Widget.h"
#include "RolUI/IPainter.h"

namespace RolUI {
    class RectWidget : public Widget {
      private:
        uint32_t _round = 0;
        uint32_t _border_width = 0;
        Color _border_color;
        Color _background_color;
        Rect _rect;

      public:
        RectWidget() noexcept {}
        RectWidget(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round = 0) noexcept
            : _rect(x, y, w, h), _round(round) {}

        ~RectWidget() override;

        RectWidget& set_x(int32_t x) noexcept;
        RectWidget& set_y(int32_t y) noexcept;
        RectWidget& set_pos(int32_t x, int32_t y) noexcept;

        RectWidget& set_width(uint32_t w) noexcept;
        RectWidget& set_heigth(uint32_t h) noexcept;
        RectWidget& set_size(uint32_t w, uint32_t h) noexcept;

        RectWidget& set_round(uint32_t r) noexcept;
        RectWidget& set_border_width(uint32_t w) noexcept;

        RectWidget& set_border_color(const Color& c) noexcept;
        RectWidget& set_background_color(const Color& c) noexcept;

        void draw(IPainter* painter) override;
    };

} // namespace RolUI
