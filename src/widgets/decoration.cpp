
#include "RolUI/IPainter.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/decoration.hpp"

namespace RolUI {
    namespace widgets {

        HSeparatorWidget::HSeparatorWidget() noexcept {}
        HSeparatorWidget::~HSeparatorWidget() {}

        void HSeparatorWidget::draw(IPainter* painter) noexcept {
            Point pos = abs_pos();
            painter->set_stroke_color(color());
            painter->set_stroke_width(width());
            painter->draw_hline(abs_pos(), size().width);
        }
        Size HSeparatorWidget::perform_layout(Constraint constraint) noexcept {
            return {constraint.max_width(), width()};
        }

        VSeparatorWidget::VSeparatorWidget() noexcept {}
        VSeparatorWidget::~VSeparatorWidget() {}

        void VSeparatorWidget::draw(IPainter* painter) noexcept {
            Point pos = abs_pos();
            painter->set_stroke_color(color());
            painter->set_stroke_width(width());
            painter->draw_vline(abs_pos(), size().height);
        }
        Size VSeparatorWidget::perform_layout(Constraint constraint) noexcept {
            return {width(), constraint.max_height()};
        }

    } // namespace widgets
} // namespace RolUI