
#include "RolUI/widgets/RectWidget.h"

namespace RolUI {

    RectWidget::~RectWidget() {}

    RectWidget& RectWidget::set_x(int32_t x) noexcept {
        _rect.x = x;
        return *this;
    }
    RectWidget& RectWidget::set_y(int32_t y) noexcept {
        _rect.y = y;
        return *this;
    }
    RectWidget& RectWidget::set_pos(int32_t x, int32_t y) noexcept {
        _rect.x = x;
        _rect.y = y;
        return *this;
    }

    RectWidget& RectWidget::set_width(uint32_t w) noexcept {
        _rect.width = w;
        return *this;
    }
    RectWidget& RectWidget::set_heigth(uint32_t h) noexcept {
        _rect.height = h;
        return *this;
    }
    RectWidget& RectWidget::set_size(uint32_t w, uint32_t h) noexcept {
        _rect.width = w;
        _rect.height = h;
        return *this;
    }

    RectWidget& RectWidget::set_round(uint32_t r) noexcept {
        _round = r;
        return *this;
    }

    RectWidget& RectWidget::set_border_width(uint32_t w) noexcept {
        _border_width = w;
        return *this;
    }

    RectWidget& RectWidget::set_border_color(const Color& c) noexcept {
        _border_color = c;
        return *this;
    }
    RectWidget& RectWidget::set_background_color(const Color& c) noexcept {
        _background_color = c;
        return *this;
    }

    void RectWidget::draw(IPainter* painter) {
        painter->set_fill_color(_background_color);
        painter->fill_roundedrect(_rect, _round);

        if (_border_width > 0) {
            int32_t hw = _border_width / 2.0f;
            int32_t x = _rect.x + hw;
            int32_t y = _rect.y + hw;
            uint32_t w = _rect.width - _border_width;
            uint32_t h = _rect.height - _border_width;
            uint32_t r = _round - hw;

            painter->set_stroke_color(_border_color);
            painter->set_stroke_width(_border_width);
            painter->draw_roundedrect(x, y, w, h, r);
        }
    }
} // namespace RolUI
