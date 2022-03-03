#pragma once

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

        Widget* sized(unsigned width, unsigned height, Widget* child);
        Widget* sized(float width, float height, Widget* child);
        Widget* sized(unsigned width, float height, Widget* child);
        Widget* sized(float width, unsigned height, Widget* child);

        template <typename WT, typename... Args>
        WT* mk_widget(Args&&... args) { return new WT(args...); }

    } // namespace widgets
} // namespace RolUI
