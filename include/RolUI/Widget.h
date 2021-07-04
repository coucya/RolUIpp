#pragma once

#include <stdint.h>

#include "Size.h"
#include "Point.h"
#include "IWidget.h"
#include "ListenerBase.h"

namespace RolUI {

    class Widget;
    class Window;

    class Widget : public IWidget, public ListenerBase {
        friend class Window;

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

        Widget* parent() noexcept { return _parent; }
        const Widget* parent() const noexcept { return _parent; }

        Window* window() noexcept { return _window; }
        const Window* window() const noexcept { return _window; }

        ChlidrenView children_view(bool reverse = false) noexcept;
        const ChlidrenView children_view(bool reverse = false) const noexcept;

        Point pos() override;
        Size size() override;

        void set_pos(const Point& pos) noexcept;
        void set_size(const Size& size) noexcept;

        void set_pos(int32_t x, int32_t y) noexcept;
        void set_size(uint32_t w, uint32_t h) noexcept;

        void set_parent(Widget* widget) noexcept;

        void add_child(Widget* widget) noexcept;
        void remove_child(Widget* widget) noexcept;

        void draw(IPainter* painter) override;

      protected:
        Point _pos;
        Size _size;

        Window* _window;
        Widget* _parent;

        IntrusiveListNode _brother;
        IntrusiveList<Widget> _children;

      private:
        void _set_window(Window* w) noexcept;
        void _set_window_for_chilren(Window* w);
    };

} // namespace RolUI
