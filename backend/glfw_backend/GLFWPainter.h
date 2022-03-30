#pragma once

#include <stdint.h>

#include "RolUI/Rect.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Color.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUIBackend {

    class GLFWWindow;

    class GLFWPainter : public RolUI::IPainter {
        friend class GLFWWindow;

      public:
        ~GLFWPainter() override;

        virtual bool load_font(const char* name, const char* filename) override;

        virtual RolUI::Size text_size(const char* text, uint32_t len) const override;
        virtual uint32_t text_glyph_pos(const char* text, uint32_t text_len, uint32_t* out_pos, uint32_t pos_len) const override;

        virtual void set_scissor(RolUI::Rect rect) override;
        virtual RolUI::Rect get_scissor() const override;

        virtual void set_font_size(uint32_t s) override;
        virtual void set_font_color(RolUI::Color color) override;
        virtual void set_font(const char* name) override;

        virtual int create_image_with_rgba(const uint8_t* data, int w, int h) override;
        virtual RolUI::Size image_size(int handle) override;
        virtual void delete_image(int handle) override;

        virtual void set_stroke_color(RolUI::Color color) override;
        virtual void set_fill_color(RolUI::Color color) override;

        virtual void set_stroke_width(uint32_t w) override;

        virtual void draw_text(RolUI::Point pos, const char* text, uint32_t len) override;

        virtual void draw_image(RolUI::Point pos, RolUI::Size size, int handle) override;

        virtual void draw_line(RolUI::Point a, RolUI::Point b) override;
        virtual void draw_rect(RolUI::Rect rect) override;
        virtual void draw_circle(RolUI::Point centre, uint32_t r) override;
        virtual void draw_ellipse(RolUI::Rect rect) override;
        virtual void draw_roundedrect(RolUI::Rect rect, uint32_t round) override;

        virtual void fill_rect(RolUI::Rect rect) override;
        virtual void fill_circle(RolUI::Point centre, uint32_t r) override;
        virtual void fill_ellipse(RolUI::Rect rect) override;
        virtual void fill_roundedrect(RolUI::Rect rect, uint32_t round) override;

      private:
        GLFWPainter(void* nvg_context);

        void* _nvg_context = nullptr;

        uint32_t _font_size = 15;
        uint32_t _font_id = 0;
        RolUI::Color _font_color;

        uint32_t _stroke_width = 1;
        RolUI::Color _stroke_color;
        RolUI::Color _fill_color;

        RolUI::Rect _scissor;
    };

} // namespace RolUIBackend
