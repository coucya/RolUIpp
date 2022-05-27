#pragma once

#include <stdint.h>

#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./Color.hpp"

#define ROLUI_LINE_CAP_BUTT 1
#define ROLUI_LINE_CAP_ROUND 2
#define ROLUI_LINE_CAP_SQUARE 3

#define ROLUI_LINE_JOIN_BEVEL 4
#define ROLUI_LINE_JOIN_ROUND 5
#define ROLUI_LINE_JOIN_MITER 6

namespace RolUI {

    class IPainter {
      public:
        virtual ~IPainter() {}

        virtual Size text_size(const char* text, uint32_t len) const = 0;
        virtual uint32_t text_glyph_pos(const char* text, uint32_t text_len,
                                        uint32_t* out_min_x, uint32_t* out_max_x, uint32_t pos_len) const = 0;

        virtual int32_t create_image(const uint8_t* data, uint32_t w, uint32_t h) = 0;
        virtual void delete_image(int32_t handle) = 0;
        virtual Size image_size(int32_t handle) = 0;

        virtual int32_t create_font(const uint8_t* data, uint32_t len) = 0;
        virtual void set_font(int32_t handle) = 0;
        virtual void set_font_size(uint32_t s) = 0;
        virtual void set_font_color(Color color) = 0;

        // virtual void clip_path(Rect rect) = 0;
        virtual void clip_rect(Rect rect) = 0;
        virtual Rect get_clip_rect() const = 0;
        virtual void save_clip() = 0;
        virtual void restore_clip() = 0;

        virtual void set_fill_color(Color color) = 0;

        virtual void set_line_color(Color color) = 0;
        virtual void set_line_width(uint32_t w) = 0;
        virtual void set_line_cap(int32_t cap) = 0;
        virtual void set_line_join(int32_t join) = 0;

        virtual void fill_text(Point pos, const char* text, uint32_t len) = 0;

        virtual void draw_image(Point pos, Size size, int handle) = 0;
        virtual void draw_line(Point a, Point b) = 0;

        virtual void stroke_rect(Rect rect) = 0;
        virtual void stroke_circle(Point centre, uint32_t r) = 0;
        virtual void stroke_ellipse(Rect rect) = 0;
        virtual void stroke_roundedrect(Rect rect, uint32_t round) = 0;

        virtual void fill_rect(Rect rect) = 0;
        virtual void fill_ellipse(Rect rect) = 0;
        virtual void fill_circle(Point centre, uint32_t r) = 0;
        virtual void fill_roundedrect(Rect rect, uint32_t round) = 0;
    };

} // namespace RolUI
