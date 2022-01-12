#pragma once

#include "../Widget.hpp"
#include "../Property.hpp"

namespace RolUI {
    namespace widget {

        class Margin : public Widget {
          public:
            Property<unsigned> top{this, 0};
            Property<unsigned> bottom{this, 0};
            Property<unsigned> left{this, 0};
            Property<unsigned> right{this, 0};

          public:
            Margin() noexcept;
            Margin(unsigned margin) noexcept;
            Margin(unsigned x, unsigned y) noexcept;
            Margin(unsigned top, unsigned bottom, unsigned left, unsigned right) noexcept;

          protected:
            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI