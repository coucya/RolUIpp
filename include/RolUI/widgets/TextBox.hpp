#pragma once

#include <cstddef>
#include <string>

#include "RolUI/Point.hpp"
#include "RolUI/Property.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/timer.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Rect.hpp"

namespace RolUI {
    namespace widget {

        class TextBox : public Widget {
          public:
            Property<unsigned> round{this, 0};
            Property<unsigned> border_width{this, 0};
            Property<Color> border_color{this, {0, 0, 0, 255}};
            Property<Color> background_color{this, {255, 255, 255, 255}};

            Property<unsigned> font_size{this, 15};
            Property<std::string> font_name{this, "default"};

            Property<Vec2i> padding{this, {3, 3}};
            Property<unsigned> cursor_width{this, 1};

            Property<Color> color{this, {0, 0, 0, 255}};
            Property<std::string> text{this};

          public:
            TextBox(Widget* parent = nullptr) noexcept;

            void cursor_to_prev_char() noexcept;
            void cursor_to_next_char() noexcept;
            void cursor_to_index(unsigned idx) noexcept;

            void set_style(const Style& style) override;

          protected:
            unsigned pos_to_index(Point pos) const noexcept;
            Point index_to_pos(unsigned idx) const noexcept;

          private:
            void _init() noexcept;
            void _init_part() noexcept;
            void _init_event_bind() noexcept;

            void _corsor_timer_func(double interval) noexcept;

          private:
            Text _text_widget;
            Rect _rect_widget;
            Widget _padding_widget;

            unsigned _cursor_index = 0;
            Point _cursor_pos = {0, 0};
            Rect _cursor_widget;
            Timer _cursor_timer;
        };

    } // namespace widget
} // namespace RolUI
