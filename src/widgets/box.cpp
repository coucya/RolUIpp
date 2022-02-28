
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/widgets/box.hpp"

namespace RolUI {
    namespace widgets {

        Box::Box(unsigned round) noexcept { this->round = round; }
        Box::~Box() {}

        void Box::draw(IPainter* painter) noexcept {
            Rect rect = abs_rect();
            painter->set_fill_color(background_color);
            painter->fill_roundedrect(rect, round);

            if (border_width > 0) {
                painter->set_stroke_color(border_color);
                painter->set_stroke_width(border_width * 2);
                painter->draw_roundedrect(rect, round);
            }

            SingleChildWidget::draw(painter);
        }

    } // namespace widgets
} // namespace RolUI
