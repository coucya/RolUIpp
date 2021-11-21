#pragma once

#include <string>

#include "RolUI/Color.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Property.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Widget.hpp"

#include "./Rect.hpp"
#include "./Text.hpp"

namespace RolUI {
    namespace widget {

        class Label : public Widget {
          public:
            Property<unsigned> round{this, 0};
            Property<unsigned> border_width{this, 0};
            Property<Color> border_color{this, {0, 0, 0, 255}};
            Property<Color> background_color{this, {255, 255, 255, 255}};

            Property<unsigned> font_size{this, 15};
            Property<Color> font_color{this, {0, 0, 0, 255}};
            Property<std::string> font_name{this, "default"};
            Property<std::string> text{this};

            Property<Vector> padding{this, {0, 0}};

          public:
            Label() noexcept;
            Label(std::string text) noexcept;
            Label(std::string text, Widget* parent) noexcept;
            ~Label();

            void adjust_size() noexcept;

            void set_style(const Style& style) override;

          private:
            void _init_part(std::string&& text) noexcept;

          private:
            widget::Rect _rect_widget;
            widget::Text _text_widget;
        };

    } // namespace widget
} // namespace RolUI
