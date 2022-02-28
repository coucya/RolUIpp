#pragma once

#include "../Color.hpp"
#include "../Property.hpp"
#include "../Rect.hpp"
#include "../Widget.hpp"
#include "../IPainter.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widget {

        class Rect : public SingleChildWidget {
          public:
            Property<unsigned> round{this, 0};
            Property<unsigned> border_width{this, 0};
            Property<Color> border_color{this, {0, 0, 0, 255}};
            Property<Color> background_color{this, {255, 255, 255, 255}};

          public:
            Rect(unsigned round = 0) noexcept;
            ~Rect() override;

          protected:
            virtual void draw(IPainter* painter) noexcept override;

          private:
        };

    } // namespace widget
} // namespace RolUI
