#pragma once

#include <stdint.h>

#include "IWidget.h"
#include "IntrusiveList.h"

namespace RolUI {

    class Window;
    class Widget;

    class WidgetBase : public IWidget {
        friend class Window;
        friend class Widget;

      public:
        WidgetBase() noexcept;
        WidgetBase(WidgetBase* parent) noexcept;

        virtual ~WidgetBase() override;

        WidgetBase* parent() noexcept { return _parent; }
        const WidgetBase* parent() const noexcept { return _parent; };

        void set_parent(WidgetBase* widget) noexcept;

        void add_child(WidgetBase* widget) noexcept;
        void remove_child(WidgetBase* widget) noexcept;

        Window* window() noexcept { return _window; }
        const Window* window() const noexcept { return _window; }

      private:
        WidgetBase* _parent;

        IntrusiveListNode _brother;
        IntrusiveList<WidgetBase> _children;

        Window* _window;

      private:
        void _set_window(Window* w) noexcept;
        void _set_window_for_chilren(Window* w);
    };

} // namespace RolUI
