#pragma once

#include <cstdio>
#include <string>

#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Property.hpp"

namespace RolUI {
    namespace widget {

        class Text : public Widget {
          public:
            Property<unsigned> font_size{this, 15};
            Property<Color> font_color{this, {0, 0, 0, 255}};
            Property<std::string> font_name{this, "default"};
            Property<std::string> text{this};

          public:
            Text(Widget* parent = nullptr) noexcept;
            ~Text() override;

            void on_draw(IPainter* painter) override;
            void set_style(const Style& style) override;

          private:
            void _update_size();
        };

    } // namespace widget
} // namespace RolUI
