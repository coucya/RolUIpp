#pragma once

#include <cstdio>
#include <string>

#include "../Widget.hpp"
#include "../IPainter.hpp"
#include "../Property.hpp"

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
            virtual void on_draw(IPainter* painter) override;
            virtual Size perlayout(Constraint constraint) override;

          private:
            void _update_size() noexcept;
            Point _index_to_pos(unsigned index) const noexcept;

            Size _size;
        };

    } // namespace widget
} // namespace RolUI
