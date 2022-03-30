#pragma once

#include <cstdio>
#include <string>
#include <vector>

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
            TextWidget() noexcept;
            TextWidget(const std::string& text) noexcept;
            ~TextWidget() override;

            unsigned pos_to_index(Point pos) const noexcept;
            Point index_to_pos(unsigned index) const noexcept;

            unsigned char_count() const noexcept;
            unsigned char_index_to_byte_index(unsigned idx) const noexcept;
            unsigned char_byte_size(unsigned idx) const noexcept;
            unsigned byte_index_to_char_index(unsigned idx) const noexcept;

            unsigned line_height() const noexcept;

          protected:
            virtual void draw(IPainter* painter) noexcept override;
            virtual Size perform_layout(Constraint constraint) noexcept override;

          private:
            void _update_size() noexcept;
            void _did_text_change() noexcept;

            Point _char_index_to_pos(unsigned index) const noexcept;

            struct CharInfo {
                int32_t codepoint = 0;
                uint32_t utf8_str_beg_idx = 0;
                uint32_t utf8_str_end_idx = 0;
                uint32_t pos_x = 0;
            };

            Size _text_size;
            std::vector<CharInfo> _chars;
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

        class TextBoxWidget : public EditableTextWidget {
          public:
            TextBoxWidget() noexcept;
            ~TextBoxWidget();

            bool handle_event(IEvent* e) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI
