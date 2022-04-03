#include "RolUI/widgets/widgets.hpp"

#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/layer.hpp"
#include "RolUI/widgets/listener.hpp"

namespace RolUI {
    namespace widgets {

        TextSpanWidget* text(const char* str, unsigned size, Color color) {
            return mk_widget<TextSpanWidget>(str)->font_size(size)->font_color(color);
        }

        ImageWidget* image(Image image) {
            return mk_widget<ImageWidget>(image);
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

            PointerListener* plw = mk_widget<PointerListener>();
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
            PointerListener* plw = mk_widget<PointerListener>();
            plw->set_child(box);
            plw->on_hover.connect([=](bool b) { box->background_color(b ? hover : normal); });
            plw->on_down.connect([=](Point) { box->background_color(press); });
            plw->on_up.connect([=](Point) { box->background_color(hover); });
            plw->on_click.connect([cb = std::move(callback)](Point) { if (cb) cb(); });
            return plw;
        }

        BoxWidget* box(unsigned round, unsigned border_width, Color border_color, Color background_color, Widget* child) {
            return (BoxWidget*)mk_widget<BoxWidget>()->round(round)->border_width(border_width)->border_color(border_color)->background_color(background_color)->set_child(child);
        }

        AlignWidget* align(float x, float y, Widget* child) {
            return (AlignWidget*)mk_widget<AlignWidget>()->align_x(x)->align_y(y)->set_child(child);
        }

        SizedBoxWidget* sized(int width, int height, Widget* child) {
            return (SizedBoxWidget*)mk_widget<SizedBoxWidget>()
                ->width(SizeUnit(width))
                ->height(SizeUnit(height))
                ->set_child(child);
        }
        SizedBoxWidget* sized(float width, float height, Widget* child) {
            return (SizedBoxWidget*)mk_widget<SizedBoxWidget>()
                ->width(SizeUnit(width))
                ->height(SizeUnit(height))
                ->set_child(child);
        }
        SizedBoxWidget* sized(int width, float height, Widget* child) {
            return (SizedBoxWidget*)mk_widget<SizedBoxWidget>()
                ->width(SizeUnit(width))
                ->height(SizeUnit(height))
                ->set_child(child);
        }
        SizedBoxWidget* sized(float width, int height, Widget* child) {
            return (SizedBoxWidget*)mk_widget<SizedBoxWidget>()
                ->width(SizeUnit(width))
                ->height(SizeUnit(height))
                ->set_child(child);
        }

        MarginWidget* margin(unsigned margin, Widget* child) {
            return (MarginWidget*)mk_widget<MarginWidget>(margin)->set_child(child);
        }
        MarginWidget* margin(unsigned x, unsigned y, Widget* child) {
            return (MarginWidget*)mk_widget<MarginWidget>(x, y)->set_child(child);
        }
        MarginWidget* margin(unsigned top, unsigned right, unsigned bottom, unsigned left, Widget* child) {
            return (MarginWidget*)mk_widget<MarginWidget>(top, bottom, left, right)->set_child(child);
        }

        StackWidget* stack() { return mk_widget<StackWidget>(); }
        DeckWidget* deck() { return mk_widget<DeckWidget>(); }

        RowWidget* row() { return mk_widget<RowWidget>(); }
        ColumnWidget* column() { return mk_widget<ColumnWidget>(); }
        RowGridWidget* row_grid() { return mk_widget<RowGridWidget>(); }
        ColumnGridWidget* column_grid() { return mk_widget<ColumnGridWidget>(); }

        PointerListener* pointer_listener(Widget* child) {
            return (PointerListener*)mk_widget<PointerListener>()->set_child(child);
        }

    } // namespace widgets
} // namespace RolUI
