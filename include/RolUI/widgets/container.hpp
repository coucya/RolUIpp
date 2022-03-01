#pragma once

#include "../Color.hpp"
#include "../Rect.hpp"
#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class Box : public SingleChildWidget {
          public:
            Property<unsigned> round{this, 0};
            Property<unsigned> border_width{this, 0};
            Property<Color> border_color{this, {0, 0, 0, 255}};
            Property<Color> background_color{this, {255, 255, 255, 255}};

          public:
            Box(unsigned round = 0) noexcept;
            ~Box() override;

          protected:
            virtual void draw(IPainter* painter) noexcept override;
        };

        class Align : public SingleChildWidget {
          public:
            Property<float> align_x{this, 0.5f};
            Property<float> align_y{this, 0.5f};

          public:
            Align() noexcept;
            Align(float x, float y) noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };

        class Margin : public SingleChildWidget {
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

            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI
