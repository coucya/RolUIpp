#pragma once

#include <functional>

#include "../Image.hpp"

#include "../Widget.hpp"
#include "./flow.hpp"
#include "./layer.hpp"
#include "./container.hpp"
#include "./listener.hpp"
#include "./Text.hpp"
#include "./Image.hpp"
#include "./Scroll.hpp"

namespace RolUI {
    namespace widgets {

        TextWidget* text(const char* str, unsigned size = 16, Color color = {0, 0, 0});

        Widget* label(const char* str, unsigned size = 16,
                      Color color = {0, 0, 0}, Color background_color = {255, 255, 255},
                      unsigned round = 0);

        ImageWidget* image(Image image);

        Widget* button(const char* str,
                       std::function<void()> callback,
                       unsigned text_size = 16,
                       Color text_color = {0, 0, 0},
                       Color normal = {240, 240, 240},
                       Color hover = {230, 230, 230},
                       Color press = {250, 250, 250},
                       unsigned round = 0);

        Widget* button(Widget* normal, Widget* hover, Widget* press,
                       std::function<void()> callback);

        BoxWidget* box(unsigned round, unsigned border_width, Color border_color, Color background_color, Widget* child);

        AlignWidget* align(float x, float y, Widget* child);

        SizedBoxWidget* sized(int width, int height, Widget* child);
        SizedBoxWidget* sized(float width, float height, Widget* child);
        SizedBoxWidget* sized(int width, float height, Widget* child);
        SizedBoxWidget* sized(float width, int height, Widget* child);

        MarginWidget* margin(unsigned margin, Widget* child);
        MarginWidget* margin(unsigned x, unsigned y, Widget* child);
        MarginWidget* margin(unsigned top, unsigned right, unsigned bottom, unsigned left, Widget* child);

        StackWidget* stack();
        DeckWidget* deck();

        RowWidget* row();
        ColumnWidget* column();
        RowGridWidget* row_grid();
        ColumnGridWidget* column_grid();

        PointerListener* pointer_listener(Widget* child);

        template <typename WT, typename... Args>
        WT* mk_widget(Args&&... args) { return new WT(args...); }

    } // namespace widgets
} // namespace RolUI
