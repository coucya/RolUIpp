
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"

namespace RolUI {
    namespace widget {

        Rect::Rect(int x, int y, unsigned w, unsigned h, unsigned round) noexcept {
            set_pos(x, y);
            set_size((int)w, (int)h);
            round = round;
        }
        Rect::~Rect() {}

        void Rect::on_draw(IPainter* painter) {
            RolUI::Rect rect_ = {{0, 0}, size()};
            painter->set_fill_color(background_color);
            painter->fill_roundedrect(rect_, round);

            if (border_width > 0) {
                painter->set_stroke_color(border_color);
                painter->set_stroke_width(border_width * 2);
                painter->draw_roundedrect(rect_, round);
            }
        }
        void Rect::set_style(const Style& style) {
            round = style.round;
            border_color = style.border_color;
            border_width = style.border_width;
            background_color = style.background_color;
        }

    } // namespace widget
} // namespace RolUI
