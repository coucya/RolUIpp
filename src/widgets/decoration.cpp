
#include "RolUI/IPainter.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/decoration.hpp"

namespace RolUI {
    namespace widgets {

        HSeparatorWidget::HSeparatorWidget() noexcept {}
        HSeparatorWidget::~HSeparatorWidget() {}

        void HSeparatorWidget::perform_draw(IPainter* painter) noexcept {
            Point pos = abs_pos();
            painter->set_line_color(color());
            painter->set_line_width(width());
            painter->draw_line(pos, Point{pos.x + size().width, pos.y});
        }
        Size HSeparatorWidget::perform_layout(Constraint constraint) noexcept {
            return {constraint.max_width(), width()};
        }

        VSeparatorWidget::VSeparatorWidget() noexcept {}
        VSeparatorWidget::~VSeparatorWidget() {}

        void VSeparatorWidget::perform_draw(IPainter* painter) noexcept {
            Point pos = abs_pos();
            painter->set_line_color(color());
            painter->set_line_width(width());
            painter->draw_line(pos, Point{pos.x, pos.y + size().height});
        }
        Size VSeparatorWidget::perform_layout(Constraint constraint) noexcept {
            return {width(), constraint.max_height()};
        }

        const ObjectType* HSeparatorWidget::object_type() const noexcept { return object_type_of<HSeparatorWidget>(); }
        const ObjectType* VSeparatorWidget::object_type() const noexcept { return object_type_of<VSeparatorWidget>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, HSeparatorWidget, Widget);
    RolUI_impl_object_type_of_with_namespace(widgets, VSeparatorWidget, Widget);

} // namespace RolUI
