#pragma once

#include <stdint.h>

#include "Rect.h"
#include "Color.h"
namespace RolUI {

    class IPainter {
      public:
        virtual ~IPainter() {}

        virtual void set_stroke_color(Color color) = 0;
        virtual void set_fill_color(Color color) = 0;

        virtual void set_stroke_width(uint32_t w) = 0;

        virtual void draw_rect(const Rect& rect) = 0;
        virtual void draw_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void draw_roundedrect(const Rect& rect, uint32_t round) = 0;
        virtual void draw_roundedrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) = 0;

        virtual void draw_ellipse(const Rect& rect) = 0;
        virtual void draw_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void draw_circle(const Point& centre, uint32_t r) = 0;
        virtual void draw_circle(int32_t x, int32_t y, uint32_t r) = 0;

        virtual void fill_rect(const Rect& rect) = 0;
        virtual void fill_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void fill_roundedrect(const Rect& rect, uint32_t round) = 0;
        virtual void fill_roundedrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) = 0;

        virtual void fill_ellipse(const Rect& rect) = 0;
        virtual void fill_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void fill_circle(const Point& centre, uint32_t r) = 0;
        virtual void fill_circle(int32_t x, int32_t y, uint32_t r) = 0;
    };

} // namespace RolUI
