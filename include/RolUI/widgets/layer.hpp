#pragma once

#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class StackWidget : public MultiChildWidget {
          public:
            Property<StackWidget, float> align_x{this, 0.0f};
            Property<StackWidget, float> align_y{this, 0.0f};

          public:
            StackWidget() noexcept;
            StackWidget(float x, float y) noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };

        class DeckWidget : public MultiChildWidget {
          public:
            Property<DeckWidget, unsigned> selected{this, 0};

          public:
            DeckWidget() noexcept;
            DeckWidget(unsigned selected) noexcept;

            Widget* get_child_by_pos(Point pos) const noexcept override;

            void draw(IPainter* painter) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI