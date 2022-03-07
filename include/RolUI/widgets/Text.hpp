#pragma once

#include <cstdio>
#include <string>

#include "../Widget.hpp"
#include "../IPainter.hpp"
#include "../Property.hpp"

#include "../events/MouseEvent.hpp"
#include "../events/CharEvent.hpp"
#include "../events/Widget_event.hpp"

namespace RolUI {
    namespace widgets {

        class TextWidget : public Widget {
          public:
            Property<TextWidget, unsigned> font_size{this, 15};
            Property<TextWidget, Color> font_color{this, {0, 0, 0, 255}};
            Property<TextWidget, std::string> font_name{this, "default"};
            Property<TextWidget, std::string> text{this};

          public:
            TextWidget(const std::string& text) noexcept;
            ~TextWidget() override;

            unsigned pos_to_index(Point pos) const noexcept;
            Point index_to_pos(unsigned index) const noexcept;
            unsigned line_height() const noexcept;

          protected:
            virtual void draw(IPainter* painter) noexcept override;
            virtual Size perform_layout(Constraint constraint) noexcept override;

          private:
            void _update_size() noexcept;

            // unsigned _byte_index_to_char_index(unsigned index) const noexcept;
            // unsigned _char_index_to_byte_index(unsigned index) const noexcept;

            Point _byte_index_to_pos(unsigned index) const noexcept;
            Point _char_index_to_pos(unsigned index) const noexcept;

            Size _size;
        };

        class EditableTextWidget : public TextWidget {
          public:
            Property<EditableTextWidget, unsigned int> cursor_index{this, 0};

            EditableTextWidget() noexcept;
            ~EditableTextWidget() override;
            bool is_blinking() const noexcept;
            void set_blink(bool blink) noexcept;

            void delete_front() noexcept;
            void delete_back() noexcept;

            void insert_char(unsigned idx, uint32_t char_) noexcept;
            void insert_str(unsigned idx, const char* str) noexcept;

            void insert_str(unsigned idx, const char* str, unsigned len) noexcept;

            void draw(IPainter* painter) noexcept override;

          private:
            void _delete_at_index(unsigned idx, unsigned len) noexcept;
            void _update_cursor_pos() noexcept;

          private:
            Point _cursor_pos;
            size_t _blink_timer_handle;
            bool _is_blinking = false;
            bool _show_cursor = false;
        };

    } // namespace widgets
} // namespace RolUI
