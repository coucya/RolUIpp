#pragma once

#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widget {

        class Stack : public MultiChildWidget {
          public:
            Property<float> align_x{this, 0.5f};
            Property<float> align_y{this, 0.5f};

          public:
            Stack() noexcept;
            Stack(float x, float y) noexcept;

            Size layout(Constraint constraint) noexcept override;
        };

        class Deck : public MultiChildWidget {
          public:
            Property<unsigned> selected{this, 0};

          public:
            Deck() noexcept;
            Deck(unsigned selected) noexcept;

            Widget* get_child_by_pos(Point pos) const noexcept override;

            void draw(IPainter* painter) noexcept override;
            Size layout(Constraint constraint) noexcept override;
        };

    } // namespace widget
} // namespace RolUI