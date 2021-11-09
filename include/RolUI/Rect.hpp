#pragma once

#include <optional>

#include "Point.hpp"
#include "RolUI/Point.hpp"
#include "Size.hpp"

namespace RolUI {

    struct Rect {
        int x, y;
        int width, height;

        Rect() noexcept : x(0), y(0), width(0), height(0) {}
        Rect(int x, int y, int w, int h) noexcept
            : x(x), y(y), width(w), height(h) {}
        Rect(const Point& pos, const Size& size) noexcept
            : x(pos.x), y(pos.y), width(size.width), height(size.height) {}

        static Rect create_with_two_point(const Point& a, const Point& b) noexcept {
            int min_x = a.x < b.x ? a.x : b.x;
            int max_x = a.x > b.x ? a.x : b.x;
            int min_y = a.y < b.y ? a.y : b.y;
            int max_y = a.y > b.y ? a.y : b.y;

            int width = max_x - min_x + 1;
            int height = max_y - min_y + 1;
            int x = min_x, y = min_y;

            return {x, y, width, height};
        }

        Point pos() const noexcept { return Point(x, y); }
        Size size() const noexcept { return Size(width, height); }

        int top() const noexcept { return y; }
        int bottom() const noexcept { return y + height; }
        int left() const noexcept { return x; }
        int right() const noexcept { return x + width; }
        int centre() const noexcept { return x + width / 2; }
        int middle() const noexcept { return y + height / 2; }

        Point left_top() const noexcept { return Point(left(), top()); }
        Point left_bottom() const noexcept { return Point(left(), bottom()); }
        Point right_top() const noexcept { return Point(right(), top()); }
        Point right_bottom() const noexcept { return Point(right(), bottom()); }

        Point centre_top() const noexcept { return Point(centre(), top()); }
        Point centre_bottom() const noexcept { return Point(centre(), bottom()); }
        Point left_middle() const noexcept { return Point(left(), middle()); }
        Point right_middle() const noexcept { return Point(right(), middle()); }

        Point centre_middle() const noexcept { return Point(centre(), middle()); }

        bool contain(int x, int y) const noexcept {
            if (x < this->x || y < this->y) return false;
            int rb_x = this->x + this->width - 1;
            int rb_y = this->y + this->height - 1;
            if (x > rb_x || y > rb_y) return false;
            return true;
        }
        bool contain(const Point& p) const noexcept {
            return contain(p.x, p.y);
        }
        bool intersect(const Rect& other) const noexcept {
            return intersected(other).has_value();
        }

        Rect united(const Rect& other) const noexcept {
            int lt_x = x < other.x ? x : other.x;
            int lt_y = y < other.y ? y : other.y;

            Point self_rb = right_bottom();
            Point other_rb = other.right_bottom();

            int rb_x = self_rb.x > other_rb.x ? self_rb.x : other_rb.x;
            int rb_y = self_rb.y > other_rb.y ? self_rb.y : other_rb.y;

            return Rect(Point(lt_x, lt_y), Point(rb_x, rb_y));
        }
        std::optional<Rect> intersected(const Rect& other) const noexcept {
            int lt_x = x > other.x ? x : other.x;
            int lt_y = y > other.y ? y : other.y;

            Point self_rb = right_bottom();
            Point other_rb = other.right_bottom();

            int rb_x = self_rb.x < other_rb.x ? self_rb.x : other_rb.x;
            int rb_y = self_rb.y < other_rb.y ? self_rb.y : other_rb.y;

            if (rb_x < lt_x || rb_y < lt_y) return {};

            return Rect(Point(lt_x, lt_y), Point(rb_x, rb_y));
        }
    };

} // namespace RolUI
