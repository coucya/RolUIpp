#pragma once

#include <stdint.h>
#include <optional>

#include "Point.hpp"
#include "Size.hpp"

namespace RolUI {

    struct Rect {
        int32_t x, y;
        uint32_t width, height;

        Rect() noexcept : x(0), y(0), width(0), height(0) {}
        Rect(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
            : x(x), y(y), width(w), height(h) {}
        Rect(const Point& pos, const Size& size) noexcept
            : x(pos.x), y(pos.y), width(size.width), height(size.height) {}
        Rect(const Point& a, const Point& b) noexcept {
            int32_t min_x = a.x < b.x ? a.x : b.x;
            int32_t max_x = a.x > b.x ? a.x : b.x;
            int32_t min_y = a.y < b.y ? a.y : b.y;
            int32_t max_y = a.y > b.y ? a.y : b.y;

            uint32_t width = max_x - min_x + 1;
            uint32_t height = max_y - min_y + 1;
            x = min_x, y = min_y;
        }

        Point pos() const noexcept { return Point(x, y); }
        Size size() const noexcept { return Size(width, height); }

        Point lefttop() const noexcept { return Point(x, y); }
        Point leftbottom() const noexcept { return Point(x, y + height - 1); }
        Point righttop() const noexcept { return Point(x + width - 1, y); }
        Point rightbottom() const noexcept { return Point(x + width - 1, y + height - 1); }

        Point centre() const noexcept { return Point(x + width / 2, y + height / 2); }

        bool is_contain_point(int32_t x, int32_t y) const noexcept {
            if (x < this->x || y < this->y) return false;
            int32_t rb_x = this->x + this->width - 1;
            int32_t rb_y = this->y + this->height - 1;
            if (x > rb_x || y > rb_y) return false;
            return true;
        }
        bool is_contain_point(const Point& p) const noexcept {
            return is_contain_point(p.x, p.y);
        }
        bool is_intersect(const Rect& other) const noexcept {
            return intersection(other).has_value();
        }

        Rect unionset(const Rect& other) const noexcept {
            int32_t lt_x = x < other.x ? x : other.x;
            int32_t lt_y = y < other.y ? y : other.y;

            Point self_rb = rightbottom();
            Point other_rb = other.rightbottom();

            int32_t rb_x = self_rb.x > other_rb.x ? self_rb.x : other_rb.x;
            int32_t rb_y = self_rb.y > other_rb.y ? self_rb.y : other_rb.y;

            return Rect(Point(lt_x, lt_y), Point(rb_x, rb_y));
        }
        std::optional<Rect> intersection(const Rect& other) const noexcept {
            int32_t lt_x = x > other.x ? x : other.x;
            int32_t lt_y = y > other.y ? y : other.y;

            Point self_rb = rightbottom();
            Point other_rb = other.rightbottom();

            int32_t rb_x = self_rb.x < other_rb.x ? self_rb.x : other_rb.x;
            int32_t rb_y = self_rb.y < other_rb.y ? self_rb.y : other_rb.y;

            if (rb_x < lt_x || rb_y < lt_y) return {};

            return Rect(Point(lt_x, lt_y), Point(rb_x, rb_y));
        }
    };

} // namespace RolUI
