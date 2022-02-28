#pragma once

#include <cstdio>
#include <string>

#include "../Widget.hpp"
#include "../IPainter.hpp"
#include "../Property.hpp"

#include "../events/MouseEvent.hpp"
#include "../events/CharEvent.hpp"
#include "../events/Widget_event.hpp"
#include "../timer.hpp"

namespace RolUI {
    namespace widget {

        class Text : public Widget {
          public:
            Property<unsigned> font_size{this, 15};
            Property<Color> font_color{this, {0, 0, 0, 255}};
            Property<std::string> font_name{this, "default"};
            Property<std::string> text{this};

          public:
            Text(const std::string& text) noexcept;
            ~Text() override;

            unsigned pos_to_index(Point pos) const noexcept;
            Point index_to_pos(unsigned index) const noexcept;
            unsigned line_height() const noexcept;

          protected:
            virtual void draw(IPainter* painter) noexcept override;
            virtual Size layout(Constraint constraint) noexcept override;

          private:
            void _update_size() noexcept;
            Point _index_to_pos(unsigned index) const noexcept;

            Size _size;
        };

        class EditableText : public Text {
          public:
            Property<unsigned int> cursor_index{this, 0};

            EditableText() noexcept;
            ~EditableText() override;
            bool cursor_blinks() const noexcept;
            void set_cursor_blinks(bool blink) noexcept;

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
            Timer _blink_timer;
            bool _show_cursor = false;
        };

    } // namespace widget
} // namespace RolUI
