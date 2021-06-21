#pragma once

#include <stdint.h>

#include "IWidget.h"
#include "IEvent.h"
#include "IEventListener.h"
#include "IntrusiveList.h"

namespace RolUI {

    class Widget;

    class WidgetEventListener : public IEventListener {
        friend class Widget;

      protected:
        IntrusiveListNode _brother;
    };

    class Widget : public IWidget, public IEventListener {
      public:
        Widget() noexcept : _parent(nullptr) {}
        Widget(Widget* parent) noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        void set_parent(Widget* widget) noexcept;

        void add_child(Widget* widget) noexcept;
        void remove_child(Widget* widget) noexcept;

        void add_listener(WidgetEventListener* listener) noexcept;
        void remove_Listener(WidgetEventListener* listener) noexcept;

        bool on_event(IEvent* event) override;

      protected:
        bool event_distribute_to_children(IEvent* event);
        bool event_distribute_to_listener(IEvent* event);

      protected:
        Widget* _parent;

        IntrusiveListNode _brother;
        IntrusiveList _children;

        IntrusiveList _listeners;
    };

} // namespace RolUI
