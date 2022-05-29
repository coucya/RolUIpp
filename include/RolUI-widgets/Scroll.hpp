#pragma once

#include <cstddef>

#include "RolUI/Property.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/sigslot.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Vector.hpp"

namespace RolUI {
    namespace widgets {

        class ScrollView : public SingleChildWidget {
          public:
            Property<ScrollView, Point> offset{this};

            Signal<Vec2i> on_scroll;

          public:
            ScrollView() noexcept;

            float widget_x_ratio() const noexcept;
            float widget_y_ratio() const noexcept;

            void scroll_by_px(int dx, int dy) noexcept;
            void scroll_by_ratio(float x, float y) noexcept;
            void scroll_x_by_px(int dx) noexcept;
            void scroll_y_by_px(int dy) noexcept;
            void scroll_x_by_ratio(float x) noexcept;
            void scroll_y_by_ratio(float y) noexcept;

            void scroll_to_px(int x, int y) noexcept;
            void scroll_to_ratio(float x, float y) noexcept;
            void scroll_x_to_px(int x) noexcept;
            void scroll_y_to_px(int y) noexcept;
            void scroll_x_to_ratio(float x) noexcept;
            void scroll_y_to_ratio(float y) noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
            void perform_draw(IPainter* painter) noexcept override;
        };

        class VScrollView : public ScrollView {
          public:
            Property<VScrollView, float> scroll_step{this, 10.0};

          public:
            VScrollView() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
            bool handle_event(IEvent* e) noexcept override;
        };

        class HScrollView : public ScrollView {
          public:
            Property<HScrollView, float> scroll_step{this, 10.0};

          public:
            HScrollView() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets

    RolUI_decl_object_type_of(widgets::ScrollView);
    RolUI_decl_object_type_of(widgets::VScrollView);
    RolUI_decl_object_type_of(widgets::HScrollView);

} // namespace RolUI
