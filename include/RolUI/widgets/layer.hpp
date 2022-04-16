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

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class DeckWidget : public MultiChildWidget {
          public:
            Property<DeckWidget, unsigned> selected{this, 0};

          public:
            DeckWidget() noexcept;
            DeckWidget(unsigned selected) noexcept;

            const ObjectType* object_type() const noexcept override;

            bool hit_test(Point pos) noexcept override;
            Widget* hit_test_children(Point pos) noexcept override;

          protected:
            void draw(IPainter* painter) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets

    RolUI_decl_object_type_of(widgets::StackWidget);
    RolUI_decl_object_type_of(widgets::DeckWidget);

} // namespace RolUI