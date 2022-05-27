#pragma once

#include <stdint.h>

#include "RolUI/Rect.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Color.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUIBackend {

    class GLFWWindow;

    using namespace RolUI;

    class GLFWPainter : public RolUI::IPainter {
        friend class GLFWWindow;

      public:
        ~GLFWPainter() override;

        virtual Size text_size(const char* text, uint32_t len) const override;
        virtual uint32_t text_glyph_pos(const char* text, uint32_t text_len,
                                        uint32_t* out_min_x, uint32_t* out_max_x, uint32_t pos_len) const override;

        virtual int32_t create_image(const uint8_t* data, uint32_t w, uint32_t h) override;
        virtual void delete_image(int32_t handle) override;
        virtual Size image_size(int32_t handle) override;

        virtual int32_t create_font(const uint8_t* data, uint32_t len) override;
        virtual void set_font(int32_t handle) override;
        virtual void set_font_size(uint32_t s) override;
        virtual void set_font_color(Color color) override;

        // virtual void clip_path(Rect rect) override;
        virtual void clip_rect(Rect rect) override;
        virtual Rect get_clip_rect() const override;
        virtual void save_clip() override;
        virtual void restore_clip() override;

        virtual void set_fill_color(Color color) override;

        virtual void set_line_color(Color color) override;
        virtual void set_line_width(uint32_t w) override;
        virtual void set_line_cap(int32_t cap) override;
        virtual void set_line_join(int32_t join) override;

        virtual void fill_text(Point pos, const char* text, uint32_t len) override;

        virtual void draw_image(Point pos, Size size, int handle) override;
        virtual void draw_line(Point a, Point b) override;

        virtual void stroke_rect(Rect rect) override;
        virtual void stroke_circle(Point centre, uint32_t r) override;
        virtual void stroke_ellipse(Rect rect) override;
        virtual void stroke_roundedrect(Rect rect, uint32_t round) override;

        virtual void fill_rect(Rect rect) override;
        virtual void fill_ellipse(Rect rect) override;
        virtual void fill_circle(Point centre, uint32_t r) override;
        virtual void fill_roundedrect(Rect rect, uint32_t round) override;

      private:
        GLFWPainter(void* nvg_context);

        void* _nvg_context = nullptr;

        int32_t _font_id = 0;
        uint32_t _font_size = 15;
        RolUI::Color _font_color;

        RolUI::Color _fill_color;

        RolUI::Color _line_color;
        uint32_t _line_width = 1;
        int32_t _line_cap = ROLUI_LINE_CAP_BUTT;
        int32_t _line_join = ROLUI_LINE_JOIN_MITER;


        RolUI::Rect _clip_rect;
    };

} // namespace RolUIBackend
