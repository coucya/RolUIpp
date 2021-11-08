
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"

namespace RolUI {
    namespace widget {

        Rect::Rect(
            int32_t x, int32_t y,
            uint32_t w, uint32_t h,
            uint32_t round) noexcept
            : _round(round) {
            set_pos(x, y);
            set_size(w, h);
        }
        Rect::~Rect() {}

        uint32_t Rect::round() const noexcept { return _round; }
        uint32_t Rect::border_width() const noexcept { return _border_width; }

        Color Rect::border_color() const noexcept { return _border_color; }
        Color Rect::background_color() const noexcept { return _background_color; }

        void Rect::set_round(uint32_t r) noexcept {
            _round = r;
        }

        void Rect::set_border_width(uint32_t w) noexcept {
            _border_width = w;
        }

        void Rect::set_border_color(const Color& c) noexcept {
            _border_color = c;
        }
        void Rect::set_background_color(const Color& c) noexcept {
            _background_color = c;
        }

        void Rect::on_draw(IPainter* painter) {
            Point pos_ = pos();
            Size size_ = size();
            painter->set_fill_color(_background_color);
            painter->fill_roundedrect(RolUI::Rect(pos_, size_), _round);

            if (_border_width > 0) {
                int32_t hw = _border_width / 2;
                int32_t x = pos_.x + hw;
                int32_t y = pos_.y + hw;
                uint32_t w = size_.width - _border_width;
                uint32_t h = size_.height - _border_width;
                uint32_t r = hw > _round ? 0 : _round - hw;

                painter->set_stroke_color(_border_color);
                painter->set_stroke_width(_border_width);
                painter->draw_roundedrect(x, y, w, h, r);
            }
        }

    } // namespace widget
} // namespace RolUI
