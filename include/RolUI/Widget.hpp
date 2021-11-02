#pragma once

#include <stdint.h>
#include <functional>
#include <vector>
#include <tuple>

#include "RolUI/IEvent.hpp"
#include "RolUI/IWidget.hpp"
#include "RolUI/IEventListener.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"

namespace RolUI {

    class Window;

    typedef bool (*EventCallbackFunc)(IEvent*);
    typedef std::function<bool(IEvent*)> EventCallback;

    class Widget : public IWidget, public IEventListener {

        friend Window;

      public:
        Widget() noexcept;
        Widget(Widget* parent) noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        Point pos() const noexcept;
        Size size() const noexcept;
        Rect rect() const noexcept;

        Point abs_pos() const noexcept;

        void set_pos(const Point& pos) noexcept;
        void set_size(const Size& size) noexcept;

        void set_pos(int32_t x, int32_t y) noexcept;
        void set_size(uint32_t w, uint32_t h) noexcept;

        Window* window() const noexcept;

        Widget* parent() const noexcept;
        Widget* real_parent() const noexcept;

        virtual size_t child_count() const noexcept;
        virtual bool is_my_child(Widget* w) const noexcept;
        virtual Widget* get_child(size_t idx) const noexcept;
        virtual void add_child(Widget* w) noexcept;
        virtual void remove_child(Widget* w) noexcept;

        virtual size_t part_count() const noexcept;
        virtual bool is_my_part(Widget* w) const noexcept;
        virtual Widget* get_part(size_t idx) const noexcept;
        virtual void add_part(Widget* w) noexcept;
        virtual void remove_part(Widget* w) noexcept;

        virtual void remove_widget(Widget* w) noexcept;
        virtual bool is_my_child_or_part(Widget* w) const noexcept;

        virtual Widget* get_child_by_pos(Point pos) const noexcept;
        virtual Widget* get_part_by_pos(Point pos) const noexcept;
        virtual Widget* get_widget_by_pos(Point pos) const noexcept;

        size_t add_listener(const EventType* et, EventCallback&& callback);
        void remove_listener(size_t handle);

        bool on_event(IEvent* e) override;

      private:
        typedef std::vector<Widget*> Childrens;
        // typedef std::deque<Widget*> Childrens;

        Widget* _get_widget(size_t idx) const noexcept;
        void _add_widget(Childrens::iterator pos, Widget* w) noexcept;
        void _remove_widget(Childrens::iterator pos) noexcept;

        Childrens::iterator _child_begin_it() const noexcept;
        Childrens::iterator _child_end_it() const noexcept;
        Childrens::iterator _part_begin_it() const noexcept;
        Childrens::iterator _part_end_it() const noexcept;

        Childrens::reverse_iterator _child_rbegin_it() const noexcept;
        Childrens::reverse_iterator _child_rend_it() const noexcept;
        Childrens::reverse_iterator _part_rbegin_it() const noexcept;
        Childrens::reverse_iterator _part_rend_it() const noexcept;

        Childrens::iterator _find_widget_it(Widget* w) const noexcept;
        Childrens::iterator _find_child_it(Widget* w) const noexcept;
        Childrens::iterator _find_part_it(Widget* w) const noexcept;

        void _set_window(Window* w) noexcept;

      private:
        Point _pos;
        Size _size;

        Widget* _parent = nullptr;
        Widget* _real_parent = nullptr;

        Window* _window = nullptr;

        size_t _part_count = 0;
        Childrens _children;

        typedef std::tuple<const EventType*, EventCallback, size_t> CallbackItem;
        size_t _event_handle = 0;
        std::vector<CallbackItem> _callbacks;
    };

} // namespace RolUI
