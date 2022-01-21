
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/widgets/Rect.hpp"

namespace RolUI {
    namespace widget {

        Rect::Rect(unsigned round) noexcept { this->round = round; }
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

            SingleChildWidget::on_draw(painter);
        }

    } // namespace widget
} // namespace RolUI
