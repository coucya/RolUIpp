
#include "RolUI/widgets/EllipseWidget.hpp"

namespace RolUI {
    namespace widget {

        EllipseWidget::EllipseWidget(
            int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
            : _border_width(0) {
            set_pos(x, y);
            set_size(w, h);
        }
        EllipseWidget::~EllipseWidget() {}

        void EllipseWidget::set_border_width(uint32_t w) noexcept {
            _border_width = w;
        }

        void EllipseWidget::set_border_color(const Color& c) noexcept {
            _border_color = c;
        }
        void EllipseWidget::set_background_color(const Color& c) noexcept {
            _background_color = c;
        }

        void EllipseWidget::on_draw(IPainter* painter) {
            painter->set_fill_color(_background_color);
            painter->fill_ellipse(pos().x, pos().y, size().width, size().height);

            if (_border_width > 0) {
                int32_t hw = _border_width / 2.0f;
                int32_t x = pos().x + hw;
                int32_t y = pos().y + hw;
                uint32_t w = size().width - _border_width;
                uint32_t h = size().height - _border_width;

                painter->set_stroke_color(_border_color);
                painter->set_stroke_width(_border_width);
                painter->draw_ellipse(x, y, w, h);
            }
        }

    } // namespace widget
} // namespace RolUI
