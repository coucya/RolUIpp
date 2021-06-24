#pragma once

#include <stdint.h>

#include "RolUI/Rect.h"
#include "RolUI/Point.h"
#include "RolUI/Color.h"
#include "RolUI/IPainter.h"

namespace RolUIBackend {

    class GLFWWindow;

    class GLFWPainter : public RolUI::IPainter {
        friend class GLFWWindow;

      public:
        ~GLFWPainter() override;

        virtual void set_stroke_color(RolUI::Color color) override;
        virtual void set_fill_color(RolUI::Color color) override;

        virtual void set_stroke_width(uint32_t w) override;

        virtual void draw_line(const RolUI::Point& a, const RolUI::Point& b) override;
        virtual void draw_rect(const RolUI::Rect& rect) override;
        virtual void draw_circle(const RolUI::Point& centre, uint32_t r) override;
        virtual void draw_ellipse(const RolUI::Rect& rect) override;
        virtual void draw_roundedrect(const RolUI::Rect& rect, uint32_t round) override;

        virtual void fill_rect(const RolUI::Rect& rect) override;
        virtual void fill_roundedrect(const RolUI::Rect& rect, uint32_t round) override;
        virtual void fill_ellipse(const RolUI::Rect& rect) override;
        virtual void fill_circle(const RolUI::Point& centre, uint32_t r) override;

      private:
        GLFWPainter(void* nvg_context);

        void* _nvg_context;
        RolUI::Color _fill_color;
        RolUI::Color _stroke_color;
        uint32_t _stroke_width;
    };

} // namespace RolUIBackend
