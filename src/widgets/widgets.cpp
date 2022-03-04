#include "RolUI/widgets/widgets.hpp"

#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/PointerListener.hpp"
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

        Widget* button(const char* str,
                       std::function<void(Point)> callback,
                       unsigned text_size, unsigned round,
                       Color text_color, Color color,
                       Color hover, Color press) {
            BoxWidget* box =
                (BoxWidget*)mk_widget<BoxWidget>()
                    ->round(round)
                    ->background_color(color)
                    ->set_child(mk_widget<AlignWidget>()
                                    ->set_child(text(str, text_size, text_color)));
            PointerListenerWidget* plw = mk_widget<PointerListenerWidget>();
            plw->set_child(box);
            plw->on_hover.connect([=](bool b) {
                if (b) {
                    box->background_color = hover;
                } else {
                    box->background_color = color;
                }
            });
            plw->on_down.connect([=](Point) {
                box->background_color = press;
            });
            plw->on_up.connect([=](Point) {
                box->background_color = hover;
            });
            plw->on_click.connect(std::move(callback));
            return plw;
        }

        Widget* sized(int width, int height, Widget* child) {
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
        Widget* sized(int width, float height, Widget* child) {
            return (new SizedBoxWidget())
                ->width(SizeUnit::pixel(width))
                ->height(SizeUnit::percentage(height))
                ->set_child(child);
        }
        Widget* sized(float width, int height, Widget* child) {
            return (new SizedBoxWidget())
                ->width(SizeUnit::percentage(width))
                ->height(SizeUnit::pixel(height))
                ->set_child(child);
        }

    } // namespace widgets
} // namespace RolUI
