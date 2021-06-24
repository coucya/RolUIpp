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
        typedef IntrusiveView<IntrusiveList::iterator, Widget, IntrusiveListNode> ChlidrenView;

      public:
        Widget() noexcept : _parent(nullptr) {}
        Widget(Widget* parent) noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        Widget* parent() noexcept { return _parent; }
        const Widget* parent() const noexcept { return _parent; }

        ChlidrenView children_view() noexcept;
        const ChlidrenView children_view() const noexcept;

        void set_parent(Widget* widget) noexcept;

        void add_child(Widget* widget) noexcept;
        void remove_child(Widget* widget) noexcept;

        void add_listener(WidgetEventListener* listener) noexcept;
        void remove_Listener(WidgetEventListener* listener) noexcept;

        bool on_event(IEvent* event) override;
        void draw(IPainter* painter) override;

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
