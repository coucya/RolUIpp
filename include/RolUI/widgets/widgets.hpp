#pragma once

#include <functional>

#include "../Image.hpp"

#include "../Widget.hpp"
#include "./Text.hpp"
#include "./Image.hpp"
#include "./Scroll.hpp"
#include "./container.hpp"
#include "./flow.hpp"
#include "./layer.hpp"

namespace RolUI {
    namespace widgets {

        Widget* text(const char* str, unsigned size = 16, Color color = {0, 0, 0});

        Widget* image(Image image);

        Widget* button(const char* str,
                       std::function<void(Point)> callback,
                       unsigned round = 0,
                       Color text_color = {0, 0, 0},
                       Color color = {240, 240, 240},
                       Color hover = {230, 230, 230},
                       Color press = {250, 250, 250});

        Widget* sized(int width, int height, Widget* child);
        Widget* sized(float width, float height, Widget* child);
        Widget* sized(int width, float height, Widget* child);
        Widget* sized(float width, int height, Widget* child);

        template <typename WT, typename... Args>
        WT* mk_widget(Args&&... args) { return new WT(args...); }

    } // namespace widgets
} // namespace RolUI
