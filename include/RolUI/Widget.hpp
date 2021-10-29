#pragma once

#include <stdint.h>

#include "Size.h"
#include "Point.h"
#include "WidgetBase.h"
#include "ListenerBase.h"

namespace RolUI {

    class Widget : public WidgetBase, public ListenerBase {
      public:
        typedef typename IntrusiveList<Widget>::View ChlidrenView;

      public:
        Widget() noexcept;
        Widget(Widget* parent) noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        Point pos() override;
        Size size() override;

        Widget* parent() noexcept;
        const Widget* parent() const noexcept;

        ChlidrenView children_view(bool reverse = false) noexcept;
        const ChlidrenView children_view(bool reverse = false) const noexcept;

        void set_pos(const Point& pos) noexcept;
        void set_size(const Size& size) noexcept;

        void set_pos(int32_t x, int32_t y) noexcept;
        void set_size(uint32_t w, uint32_t h) noexcept;

        void draw(IPainter* painter) override;

      protected:
        Point _pos;
        Size _size;
    };

} // namespace RolUI
