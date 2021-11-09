
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"

namespace RolUI {
    namespace widget {

        Rect::Rect(
            int x, int y,
            unsigned w, unsigned h,
            unsigned round) noexcept
            : _round(round) {
            set_pos(x, y);
            set_size((int)w, (int)h);
        }
        Rect::~Rect() {}

        unsigned Rect::round() const noexcept { return _round; }
        unsigned Rect::border_width() const noexcept { return _border_width; }

        Color Rect::border_color() const noexcept { return _border_color; }
        Color Rect::background_color() const noexcept { return _background_color; }

        void Rect::set_round(unsigned r) noexcept {
            _round = r;
        }

        void Rect::set_border_width(unsigned w) noexcept {
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
                int hw = _border_width / 2;
                int x = pos_.x + hw;
                int y = pos_.y + hw;
                unsigned w = size_.width - _border_width;
                unsigned h = size_.height - _border_width;
                unsigned r = hw > _round ? 0 : _round - hw;

                painter->set_stroke_color(_border_color);
                painter->set_stroke_width(_border_width);
                painter->draw_roundedrect(x, y, w, h, r);
            }
        }

    } // namespace widget
} // namespace RolUI
