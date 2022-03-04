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

        Widget* label(const char* str, unsigned size, Color color,
                      Color background_color, unsigned round) {
            return mk_widget<BoxWidget>()
                ->round(round)
                ->background_color(background_color)
                ->set_child(
                    mk_widget<AlignWidget>()
                        ->set_child(text(str, size, color)));
        }

        Widget* button(Widget* normal, Widget* hover, Widget* press,
                       std::function<void()> callback) {
            DeckWidget* dw = mk_widget<DeckWidget>();
            dw->add_child(normal)->add_child(hover)->add_child(press);

            PointerListenerWidget* plw = mk_widget<PointerListenerWidget>();
            plw->set_child(dw);

            plw->on_hover.connect([=](bool b) { dw->selected(b ? 1 : 0); });
            plw->on_down.connect([=](Point) { dw->selected(2); });
            plw->on_up.connect([=](Point) { dw->selected(1); });
            plw->on_click.connect([cb = std::move(callback)](Point) { if(cb) cb(); });
            return plw;
        }

        Widget* button(const char* str,
                       std::function<void()> callback,
                       unsigned text_size, Color text_color,
                       Color normal, Color hover, Color press, unsigned round) {
            BoxWidget* box = mk_widget<BoxWidget>();
            box->round(round)
                ->background_color(normal)
                ->set_child(mk_widget<AlignWidget>()
                                ->set_child(text(str, text_size, text_color)));
            PointerListenerWidget* plw = mk_widget<PointerListenerWidget>();
            plw->set_child(box);
            plw->on_hover.connect([=](bool b) { box->background_color(b ? hover : normal); });
            plw->on_down.connect([=](Point) { box->background_color(press); });
            plw->on_up.connect([=](Point) { box->background_color(hover); });
            plw->on_click.connect([cb = std::move(callback)](Point) { if (cb) cb(); });
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
