#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>
#include <tuple>
#include <type_traits>

#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./IEvent.hpp"
#include "./sigslot/Signal.hpp"
#include "./sigslot/Slot.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IPainter;
    class Application;

    class SingleChildWidget;
    class MultiChildWidget;

    typedef bool (*EventCallbackFunc)(IEvent*);
    typedef std::function<bool(IEvent*)> EventCallback;

    bool send_event(Widget* w, IEvent* e);

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

    class Widget : public HasSlot {
        friend class Window;
        friend class Application;

        friend class SingleChildWidget;
        friend class MultiChildWidget;

        friend void set_pos(Widget*, Point);

      public:
        Widget() noexcept;
        // Widget(Widget* parent) noexcept;

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

        Widget* parent() const noexcept;

        Size layout(Constraint constraint) noexcept;

        virtual Widget* get_child_by_pos(Point pos) const noexcept;

        virtual bool handle_event(IEvent* e) noexcept;

        virtual void draw(IPainter* painter) noexcept;

        virtual Size perform_layout(Constraint constraint) noexcept;

        virtual void update_pos() noexcept;

        virtual bool hit_test(Point pos) const noexcept;

      private:
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

        Widget* child() const noexcept;
        void set_child(Widget* child) noexcept;

        virtual Widget* get_child_by_pos(Point pos) const noexcept override;

        virtual void draw(IPainter* painter) noexcept override;
        virtual Size perform_layout(Constraint constraint) noexcept override;
        virtual void update_pos() noexcept override;

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

        int child_count() const noexcept;

        Widget* child(int index) const noexcept;

        void add_child(Widget* child) noexcept;
        void set_child(int index, Widget* child) noexcept;

        void remove_child(Widget* child) noexcept;
        void remove_child(int index) noexcept;

        virtual Widget* get_child_by_pos(Point pos) const noexcept override;

        virtual void draw(IPainter* painter) noexcept override;

        virtual Size perform_layout(Constraint constraint) noexcept override;
        virtual void update_pos() noexcept override;
    };

} // namespace RolUI
