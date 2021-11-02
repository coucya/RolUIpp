
#include "RolUI/widgets/RectWidget.hpp"

namespace RolUI {

    RectWidget::RectWidget(
        int32_t x, int32_t y,
        uint32_t w, uint32_t h,
        uint32_t round) noexcept
        : _round(round) {
        set_pos(x, y);
        set_size(w, h);
    }
    RectWidget::~RectWidget() {}

    void RectWidget::set_round(uint32_t r) noexcept {
        _round = r;
    }

    void RectWidget::set_border_width(uint32_t w) noexcept {
        _border_width = w;
    }

    void RectWidget::set_border_color(const Color& c) noexcept {
        _border_color = c;
    }
    void RectWidget::set_background_color(const Color& c) noexcept {
        _background_color = c;
    }

    void RectWidget::on_draw(IPainter* painter) {
        painter->set_fill_color(_background_color);
        painter->fill_roundedrect(Rect(pos(), size()), _round);

        if (_border_width > 0) {
            int32_t hw = _border_width / 2.0f;
            int32_t x = pos().x + hw;
            int32_t y = pos().y + hw;
            uint32_t w = size().width - _border_width;
            uint32_t h = size().height - _border_width;
            uint32_t r = _round - hw;

            painter->set_stroke_color(_border_color);
            painter->set_stroke_width(_border_width);
            painter->draw_roundedrect(x, y, w, h, r);
        }
    }
} // namespace RolUI
