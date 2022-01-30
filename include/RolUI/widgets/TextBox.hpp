#pragma once

#include <cstddef>
#include <string>

#include "../Point.hpp"
#include "../Property.hpp"
#include "../Vector.hpp"
#include "../Widget.hpp"
#include "../timer.hpp"
#include "../widgets/Text.hpp"
#include "../widgets/Rect.hpp"

namespace RolUI {
    namespace widget {

        class TextBox : public Widget {
          public:
            Property<unsigned> font_size{this, 15};
            Property<std::string> font_name{this, "default"};

            Property<unsigned> cursor_width{this, 1};

            Property<Color> color{this, {0, 0, 0, 255}};
            Property<std::string> text{this};

          public:
            TextBox(Widget* parent = nullptr) noexcept;

            unsigned cursor_index() const noexcept;
            void cursor_to_prev_char() noexcept;
            void cursor_to_next_char() noexcept;
            void cursor_to_index(unsigned idx) noexcept;

            void insert_at_index(unsigned idx, const char* str, unsigned len) noexcept;

            void delete_at_index(unsigned idx) noexcept;
            void delete_cursor_prev() noexcept;
            void delete_cursor_next() noexcept;

            void adjust_size() noexcept;

          protected:
            unsigned pos_to_index(Point pos) const noexcept;
            Point index_to_pos(unsigned idx) const noexcept;

          private:
            void _init() noexcept;
            void _init_part() noexcept;
            void _init_event_bind() noexcept;

            bool _insert_at_index(unsigned idx, const char* str, unsigned len) noexcept;
            bool _delete_at_index(unsigned idx, unsigned len) noexcept;

            unsigned _pos_to_index(Point pos) const noexcept;
            Point _index_to_pos(unsigned idx) const noexcept;

            unsigned _pos_to_index__text_widget(Point pos) const noexcept;
            Point _index_to_pos__text_widget(unsigned idx) const noexcept;
            Point _index_to_pos__text_widget__utf8(unsigned idx) const noexcept;

            void _cursor_to_index(unsigned idx) noexcept;
            void _update_text_pos() noexcept;

            void _cursor_timer_func(double interval) noexcept;

          private:
            Text _text_widget;
            Rect _rect_widget;
            Widget _padding_widget;

            unsigned _codepoint_count = 0;

            unsigned _cursor_index = 0;
            Point _cursor_pos = {0, 0};

            Rect _cursor_widget;
            Timer _cursor_timer;
        };

    } // namespace widget
} // namespace RolUI
