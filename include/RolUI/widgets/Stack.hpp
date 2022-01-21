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

    } // namespace widget
} // namespace RolUI