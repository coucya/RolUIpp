#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>
#include <tuple>

#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/IWidget.hpp"
#include "RolUI/IEventListener.hpp"
#include "RolUI/WidgetState.hpp"
#include "RolUI/Style.hpp"
#include "RolUI/sigslot/Signal.hpp"
#include "RolUI/sigslot/Slot.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IPainter;

    typedef bool (*EventCallbackFunc)(IEvent*);
    typedef std::function<bool(IEvent*)> EventCallback;

    bool send_event(Widget* w, IEvent* e);

    enum class RelativeTarget {
        parent,
        prev,
        target,
    };
    enum class AnchorPoint {
        left_top,
        left_bottom,
        right_top,
        right_bottom,
        centre_top,
        centre_bottom,
        left_middle,
        right_middle,
        centre_middle,
    };
    enum class SizeMode {
        none,
        relative,
    };

    class Widget : public IWidget, public IEventListener, public HasSlot {

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

        Point real_pos() const noexcept;
        Point rela_pos() const noexcept;

        Size visual_size() const noexcept;

        Point abs_pos() const noexcept;
        Rect abs_rect() const noexcept;

        Rect visual_rect() const noexcept;

        void set_pos(const Point& pos) noexcept;
        void set_size(const Size& size) noexcept;

        void set_pos(int x, int y) noexcept;
        void set_size(int w, int h) noexcept;

        void move(int x, int y) noexcept;
        void move(Point offset) noexcept;

        void set_pos_target(Widget* target) noexcept;
        void set_pos_relative(
            RelativeTarget relative,
            AnchorPoint target = AnchorPoint::left_top,
            AnchorPoint self = AnchorPoint::left_top) noexcept;

        void set_size_target(Widget* target) noexcept;
        void set_size_relative(RelativeTarget relative, SizeMode size_mode = SizeMode::none) noexcept;

        Point anchor_point() const noexcept;
        Point anchor_point(AnchorPoint anchor_point) const noexcept;

        Widget* pos_target() const noexcept;
        Widget* size_target() const noexcept;

        AnchorPoint self_anchor_point() const noexcept;
        AnchorPoint target_anchor_point() const noexcept;
        SizeMode size_mode() const noexcept;

        WidgetState state() const noexcept;

        bool is_enable() const noexcept;
        void set_enable(bool b) noexcept;

        bool focusable() const noexcept;
        void set_focusable(bool b) noexcept;

        bool is_focus() const noexcept;
        void set_focus() noexcept;

        Window* window() const noexcept;

        Widget* parent() const noexcept;

        bool is_child() const noexcept;
        bool is_part() const noexcept;

        size_t child_index() const noexcept;
        size_t part_index() const noexcept;

        virtual size_t child_count() const noexcept;
        virtual Widget* get_child(size_t idx) const noexcept;
        virtual void add_child(Widget* w) noexcept;
        virtual void remove_child(Widget* w) noexcept;

        virtual size_t part_count() const noexcept;
        virtual Widget* get_part(size_t idx) const noexcept;
        virtual void add_part(Widget* w) noexcept;
        virtual void remove_part(Widget* w) noexcept;

        virtual void remove_widget(Widget* w) noexcept;

        virtual Widget* get_child_by_pos(Point pos) const noexcept;
        virtual Widget* get_part_by_pos(Point pos) const noexcept;
        virtual Widget* get_widget_by_pos(Point pos) const noexcept;

        size_t add_listener(const EventType* et, EventCallback&& callback);
        void remove_listener(size_t handle);

        void set_style(const Style& style) override;

        bool on_event(IEvent* e) override;
        void on_draw(IPainter* painter) override;

      public:
        Signal<Point> on_pos_change;
        Signal<Size> on_size_change;
        Signal<> on_focus;

      private:
        typedef std::vector<Widget*> Childrens;

        void _init_event_bind() noexcept;

        Widget* _prev_widget() noexcept;

        void _update_child_index(size_t begin = 0) noexcept;

        void _update_size() noexcept;
        void _update_pos() noexcept;
        bool _update_size_and_pos() noexcept;
        void _update_child_size_and_pos() noexcept;

        void _set_state(WidgetState s) noexcept;
        void _clear_state(WidgetState s) noexcept;
        void _reset_state() noexcept;

        Widget* _get_widget(size_t idx) const noexcept;
        void _add_widget(Childrens::iterator pos, Widget* w) noexcept;
        void _remove_widget(Childrens::iterator pos) noexcept;

        void _do_attach_tree(Widget* new_parent, size_t idx) noexcept;
        void _do_detached_tree(Widget* old_parent, size_t idx) noexcept;

        void _do_parent_change(Widget* old, Widget* new_) noexcept;
        void _do_window_change(Window* old, Window* new_) noexcept;

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

      private:
        size_t _index = 0;

        Widget* _parent = nullptr;

        Window* _window = nullptr;

        size_t _part_count = 0;
        Childrens _children;

        bool _focusable = false;
        WidgetState _widget_state = WIDGET_STATE_DEFAULT;

        Point _real_pos, _rela_pos;
        Size _real_size, _rela_size;

        Widget* _pos_target = nullptr;
        RelativeTarget _pos_relative = RelativeTarget::parent;
        AnchorPoint _self_anchor_point = AnchorPoint::left_top;
        AnchorPoint _target_anchor_point = AnchorPoint::left_top;

        Widget* _size_target = nullptr;
        RelativeTarget _size_relative = RelativeTarget::parent;
        SizeMode _size_mode = SizeMode::none;

        bool _is_update = true;

        typedef std::tuple<const EventType*, EventCallback, size_t> CallbackItem;
        size_t _event_handle = 0;
        std::vector<CallbackItem> _callbacks;
    };

} // namespace RolUI
