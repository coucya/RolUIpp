#pragma once

#include "RolUI/Color.hpp"
#include "RolUI/Property.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Rect : public Widget {
          public:
            Property<unsigned> round{this, 0};
            Property<unsigned> border_width{this, 0};
            Property<Color> border_color{this, {0, 0, 0, 255}};
            Property<Color> background_color{this, {255, 255, 255, 255}};

          public:
            Rect(Widget* parent = nullptr) noexcept;
            Rect(Widget* parent, int x, int y, unsigned w, unsigned h,
                 unsigned round = 0) noexcept;

            ~Rect() override;

            void on_draw(IPainter* painter) override;

            void set_style(const Style& style) override;

          private:
        };

    } // namespace widget
} // namespace RolUI
