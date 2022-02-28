
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/widgets/Rect.hpp"

namespace RolUI {
    namespace widget {

        Rect::Rect(unsigned round) noexcept { this->round = round; }
        Rect::~Rect() {}

        void Rect::draw(IPainter* painter) noexcept {
            RolUI::Rect rect = abs_rect();
            painter->set_fill_color(background_color);
            painter->fill_roundedrect(rect, round);

            if (border_width > 0) {
                painter->set_stroke_color(border_color);
                painter->set_stroke_width(border_width * 2);
                painter->draw_roundedrect(rect, round);
            }

            SingleChildWidget::draw(painter);
        }

    } // namespace widget
} // namespace RolUI
