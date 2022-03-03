#include "RolUI/widgets/widgets.hpp"

#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/layer.hpp"

namespace RolUI {
    namespace widgets {

        Widget* text(const char* str, unsigned size, Color color) {
            return (new TextWidget(str))->font_size(size)->font_color(color);
        }

        Widget* image(Image image) {
            return (new ImageWidget(image));
        }

        Widget* sized(unsigned width, unsigned height, Widget* child) {
            return (new SizedBoxWidget())
                ->width(SizeUnit::pixel(width))
                ->height(SizeUnit::pixel(height))
                ->set_child(child);
        }
        Widget* sized(float width, float height, Widget* child) {
            return (new SizedBoxWidget())
                ->width(SizeUnit::percentage(width))
                ->height(SizeUnit::percentage(height))
                ->set_child(child);
        }
        Widget* sized(unsigned width, float height, Widget* child) {
            return (new SizedBoxWidget())
                ->width(SizeUnit::pixel(width))
                ->height(SizeUnit::percentage(height))
                ->set_child(child);
        }
        Widget* sized(float width, unsigned height, Widget* child) {
            return (new SizedBoxWidget())
                ->width(SizeUnit::percentage(width))
                ->height(SizeUnit::pixel(height))
                ->set_child(child);
        }

    } // namespace widgets
} // namespace RolUI
