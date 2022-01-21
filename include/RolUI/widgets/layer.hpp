#pragma once

#include "../Property.hpp"
#include "./container.hpp"

namespace RolUI {
    namespace widget {

        class Stack : public MultiChildWidget {
          public:
            Property<float> align_x{this, 0.5f};
            Property<float> align_y{this, 0.5f};

          public:
            Stack() noexcept;
            Stack(float x, float y) noexcept;

            Size perlayout(Constraint constraint) override;
        };

        class Deck : public MultiChildWidget {
          public:
            Property<unsigned> selected{this, 0};

          public:
            Deck() noexcept;
            Deck(unsigned selected) noexcept;

            void on_draw(IPainter* painter) override;
            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI