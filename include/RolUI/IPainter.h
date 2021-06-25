#pragma once

#include <stdint.h>

#include "Rect.h"
#include "Color.h"
namespace RolUI {

    class IPainter {
      public:
        virtual ~IPainter() {}

        virtual bool load_font(const char* name, const char* fileName) = 0;

        virtual void set_font_size(uint32_t s) = 0;
        virtual void set_font_color(Color color) = 0;
        virtual void set_font(const char* name) = 0;

        virtual void set_stroke_color(Color color) = 0;
        virtual void set_fill_color(Color color) = 0;

        virtual void set_stroke_width(uint32_t w) = 0;

        virtual void draw_text(const Point& pos, const char* text, uint32_t len) = 0;

        virtual void draw_text(int32_t x, int32_t y, const char* text, uint32_t len) {
            draw_text(Point(x, y), text, len);
        }
        virtual void draw_text(const Point& pos, const char* text) {
            uint32_t len = 0;
            const char* text_it = text;
            while (*(text_it++) != '\0') len++;

            draw_text(pos, text, len);
        }
        virtual void draw_text(int32_t x, int32_t y, const char* text) {
            draw_text(Point(x, y), text);
        }

        virtual void draw_line(const Point& a, const Point& b) = 0;
        virtual void draw_rect(const Rect& rect) = 0;
        virtual void draw_circle(const Point& centre, uint32_t r) = 0;
        virtual void draw_ellipse(const Rect& rect) = 0;
        virtual void draw_roundedrect(const Rect& rect, uint32_t round) = 0;

        virtual void fill_rect(const Rect& rect) = 0;
        virtual void fill_roundedrect(const Rect& rect, uint32_t round) = 0;
        virtual void fill_ellipse(const Rect& rect) = 0;
        virtual void fill_circle(const Point& centre, uint32_t r) = 0;

        virtual void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
            draw_line(Point(x1, y1), Point(x2, y2));
        }
        virtual void draw_hline(const Point& a, uint32_t len) { draw_line(a, Point(a.x + len, a.y)); }
        virtual void draw_vline(const Point& a, uint32_t len) { draw_line(a, Point(a.x, a.y + len)); }
        virtual void draw_hline(int32_t x, int32_t y, uint32_t len) { draw_line(Point(x, y), Point(x + len, y)); }
        virtual void draw_vline(int32_t x, int32_t y, uint32_t len) { draw_line(Point(x, y), Point(x, y + len)); }

        virtual void draw_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) { draw_rect(Rect(x, y, w, h)); }

        virtual void draw_roundedrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) {
            draw_roundedrect(Rect(x, y, w, h), round);
        }

        virtual void draw_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) { draw_ellipse(Rect(x, y, w, h)); }

        virtual void draw_circle(int32_t x, int32_t y, uint32_t r) { draw_circle(Point(x, y), r); }

        virtual void fill_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) { fill_rect(Rect(x, y, w, h)); }

        virtual void fill_roundedrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) {
            fill_roundedrect(Rect(x, y, w, h), round);
        }

        virtual void fill_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) { fill_ellipse(Rect(x, y, w, h)); }

        virtual void fill_circle(int32_t x, int32_t y, uint32_t r) { fill_circle(Point(x, y), r); }
    };

} // namespace RolUI
