
#pragma once

#include "../Color.hpp"
#include "../Rect.hpp"
#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class HSeparatorWidget : public Widget {
          public:
            Property<HSeparatorWidget, Color> color{this, Color{0, 0, 0, 255}};
            Property<HSeparatorWidget, int> width{this, 1};

          public:
            HSeparatorWidget() noexcept;
            ~HSeparatorWidget() override;

            const ObjectType* object_type() const noexcept override;

          protected:
            void perform_draw(IPainter* painter) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class VSeparatorWidget : public Widget {
          public:
            Property<VSeparatorWidget, Color> color{this, Color{0, 0, 0, 255}};
            Property<VSeparatorWidget, int> width{this, 1};

          public:
            VSeparatorWidget() noexcept;
            ~VSeparatorWidget() override;

            const ObjectType* object_type() const noexcept override;

          protected:
            void perform_draw(IPainter* painter) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets

    RolUI_decl_object_type_of(widgets::HSeparatorWidget);
    RolUI_decl_object_type_of(widgets::VSeparatorWidget);

} // namespace RolUI