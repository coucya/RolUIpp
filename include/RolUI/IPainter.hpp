#pragma once

#include <stdint.h>

#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./Color.hpp"

namespace RolUI {

    class IPainter {
      public:
        virtual ~IPainter() {}

        virtual bool load_font(const char* name, const char* fileName) = 0;

        virtual Size text_size(const char* text, uint32_t len) const = 0;

        virtual int create_image_with_rgba(const uint8_t* data, int w, int h) = 0;
        virtual void delete_image(int handle) = 0;
        virtual Size image_size(int handle) = 0;

        virtual void set_base_pos(Point pos) = 0;
        virtual void scissor(Rect rect) = 0;

        virtual void set_font_size(uint32_t s) = 0;
        virtual void set_font_color(Color color) = 0;
        virtual void set_font(const char* name) = 0;

        virtual void set_stroke_color(Color color) = 0;
        virtual void set_fill_color(Color color) = 0;

        virtual void set_stroke_width(uint32_t w) = 0;

        virtual void draw_text(Point pos, const char* text, uint32_t len) = 0;

        virtual Size text_size(const char* text) const {
            const char* it = text;
            while (*it != '\0') it++;
            return text_size(text, it - text);
        }

        virtual void draw_image(Point pos, Size size, int handle) = 0;

        virtual void draw_line(Point a, Point b) = 0;
        virtual void draw_rect(Rect rect) = 0;
        virtual void draw_circle(Point centre, uint32_t r) = 0;
        virtual void draw_ellipse(Rect rect) = 0;
        virtual void draw_roundedrect(Rect rect, uint32_t round) = 0;

        virtual void fill_rect(Rect rect) = 0;
        virtual void fill_roundedrect(Rect rect, uint32_t round) = 0;
        virtual void fill_ellipse(Rect rect) = 0;
        virtual void fill_circle(Point centre, uint32_t r) = 0;

        virtual void draw_hline(Point a, uint32_t len) {
            draw_line(a, Point(a.x + len, a.y));
        }
        virtual void draw_vline(Point a, uint32_t len) {
            draw_line(a, Point(a.x, a.y + len));
        }
    };

} // namespace RolUI
