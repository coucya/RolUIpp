#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>
#include <tuple>
#include <type_traits>
#include <limits>

#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./Object.hpp"
#include "./IEvent.hpp"
#include "./sigslot.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IPainter;
    class Application;

    class SingleChildWidget;
    class MultiChildWidget;

    void set_pos(Widget* w, Point pos);

    class Constraint {
      public:
        static Constraint zero() noexcept { return Constraint{}; }
        static Constraint zero_to(Size max) noexcept { return Constraint({0, 0}, max); }
        static Constraint zero_to(int w, int h) noexcept { return Constraint(0, 0, w, h); }
        static Constraint limit(Size s) noexcept { return Constraint(s, s); }
        static Constraint limit(int w, int h) noexcept { return Constraint(w, h, w, h); }
        static Constraint limit(Size min, Size max) noexcept { return Constraint{min, max}; }
        static Constraint limit(int min_w, int min_h, int max_w, int max_h) noexcept {
            return Constraint(min_w, min_h, max_w, max_h);
        }
        static Constraint unlimit() noexcept {
            int imax = std::numeric_limits<int>::max();
            return Constraint(0, 0, imax, imax);
        }

      public:
        Constraint() noexcept : _min(0, 0), _max(0, 0) {}
        Constraint(Size min, Size max) noexcept : _min(min), _max(max) {}
        Constraint(int min_w, int min_h, int max_w, int max_h) noexcept
            : _min(min_w, min_h), _max(max_w, max_h) {}

        Size min() const noexcept { return _min; }
        Size max() const noexcept { return _max; }

        int max_width() const noexcept { return _max.width; }
        int max_height() const noexcept { return _max.height; }
        int min_width() const noexcept { return _min.width; }
        int min_height() const noexcept { return _min.height; }

      private:
        Size _min;
        Size _max;
    };

    class Widget : public Object, public HasSlot {
        friend class Window;
        friend class Application;

        friend class SingleChildWidget;
        friend class MultiChildWidget;

        friend void set_pos(Widget*, Point);
        friend bool send_event(Widget*, IEvent*);

      public:
        Widget() noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        Point pos() const noexcept;
        Size size() const noexcept;
        Rect rect() const noexcept;

        Point abs_pos() const noexcept;
        Rect abs_rect() const noexcept;

        bool opaque() const noexcept;
        bool is_hit() const noexcept;
        bool mounted() const noexcept;
        unsigned depth() const noexcept;
        Widget* parent() const noexcept;

        void set_opaque(bool val) noexcept;

        void mark_hit() noexcept;
        void clear_hit() noexcept;
        void clear_hit_self() noexcept;

        Size layout(Constraint constraint) noexcept;
        void draw(IPainter* painter) noexcept;

        void update_pos() noexcept;

        const ObjectType* object_type() const noexcept override;

        virtual int child_count() const noexcept;
        virtual Widget* child(int index = 0) const noexcept;
        virtual void set_child(Widget* child, int index = 0) noexcept;
        virtual void rm_child(int index = 0) noexcept;

        virtual bool hit_test(Point pos) noexcept;
        virtual bool hit_test_self(Point pos) noexcept;
        virtual Widget* hit_test_children(Point pos) noexcept;

        virtual bool handle_event(IEvent* e) noexcept;

        virtual Size perform_layout(Constraint constraint) noexcept;
        virtual void perform_draw(IPainter* painter) noexcept;

      private:
        void _mount(Widget* parent) noexcept;
        void _unmount() noexcept;
        void _attach() noexcept;
        void _detach() noexcept;

      private:
        bool _opaque = false;
        bool _is_hit = false;
        bool _mounted = false;
        unsigned _depth = 0;
        Widget* _parent = nullptr;

        Point _pos;
        Size _size;
        Point _abs_pos;
    };

    class SingleChildWidget : public Widget {
      private:
        Widget* _child = nullptr;

      public:
        SingleChildWidget() noexcept;

        const ObjectType* object_type() const noexcept override;

        int child_count() const noexcept override;
        Widget* child(int index = 0) const noexcept override;
        void set_child(Widget* child, int index = 0) noexcept override;
        void rm_child(int index = 0) noexcept override;

      protected:
        template <typename F,
                  typename = std::enable_if_t<std::is_invocable_r_v<Point, F, Size>>>
        Size layout_child(Constraint constraint, F&& f) noexcept {
            if (child()) {
                Size child_size = child()->layout(constraint);
                Point child_pos = f(child_size);
                RolUI::set_pos(child(), child_pos);
                return child_size;
            }
            return {0, 0};
        }
    };

    class MultiChildWidget : public Widget {
      private:
        typedef std::vector<Widget*> Children;
        Children _children;

      public:
        MultiChildWidget() noexcept;

        int child_count() const noexcept override;
        Widget* child(int index) const noexcept override;
        void set_child(Widget* child, int index) noexcept override;

        const ObjectType* object_type() const noexcept override;

        virtual void add_child(Widget* child) noexcept;
        virtual void insert_child(int index, Widget* child) noexcept;

        void rm_child(int index) noexcept override;
        void rm_child(Widget* child) noexcept;
        void rm_child_all() noexcept;
    };

    RolUI_decl_object_type_of(Widget);
    RolUI_decl_object_type_of(SingleChildWidget);
    RolUI_decl_object_type_of(MultiChildWidget);

    template <typename T>
    using enable_if_invocable_Widget_p_t = typename std::enable_if<std::is_invocable_r_v<void, T, Widget*>>::type;
    template <typename T>
    using enable_if_invocable_int_Widget_p_t = typename std::enable_if<std::is_invocable_r_v<void, T, int, Widget*>>::type;

    template <typename F>
    void visit_child(Widget* w, F&& f, bool reverse = false, enable_if_invocable_Widget_p_t<F>* = nullptr) noexcept {
        if (!w) return;
        if (reverse == false)
            for (int i = 0, c = w->child_count(); i < c; i++)
                f(w->child(i));
        else
            for (int i = w->child_count() - 1; i >= 0; i--)
                f(w->child(i));
    }
    template <typename F>
    void visit_child(Widget* w, F&& f, bool reverse = false, enable_if_invocable_int_Widget_p_t<F>* = nullptr) noexcept {
        if (!w) return;
        if (reverse == false)
            for (int i = 0, c = w->child_count(); i < c; i++)
                f(i, w->child(i));
        else
            for (int i = w->child_count() - 1; i >= 0; i--)
                f(i, w->child(i));
    }

    template <typename F, typename = enable_if_invocable_Widget_p_t<F>>
    void visit_tree(Widget* w, F&& f, bool reverse = false, bool preorder = true) noexcept {
        if (w) {
            if (preorder) f(w);
            if (reverse == false)
                for (int i = 0, c = w->child_count(); i < c; i++)
                    visit_tree(w->child(i), std::forward<F>(f), reverse, preorder);
            else
                for (int i = w->child_count() - 1; i >= 0; i--)
                    visit_tree(w->child(i), std::forward<F>(f), reverse, preorder);
            if (!preorder) f(w);
        }
    }

} // namespace RolUI
