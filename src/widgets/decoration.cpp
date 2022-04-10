
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

        const ObjectType* HSeparatorWidget::object_type() const noexcept { return object_type_of<HSeparatorWidget>(); }
        const ObjectType* VSeparatorWidget::object_type() const noexcept { return object_type_of<VSeparatorWidget>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, HSeparatorWidget, Widget);
    RolUI_impl_object_type_of_with_namespace(widgets, VSeparatorWidget, Widget);

} // namespace RolUI