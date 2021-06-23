#pragma once

#include <stdint.h>

#include "RolUI/Rect.h"
#include "RolUI/Point.h"
#include "RolUI/IPainter.h"

namespace RolUIBackend {

    class GLFWWindow;

    class GLFWPainter : public RolUI::IPainter {
        friend class GLFWWindow;

      public:
        GLFWPainter();
        ~GLFWPainter() override;

        virtual void draw_rect(const RolUI::Rect& rect) override;
        virtual void draw_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) override;

        virtual void draw_roundrect(const RolUI::Rect& rect, uint32_t round) override;
        virtual void draw_roundrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) override;

        virtual void draw_ellipse(const RolUI::Rect& rect) override;
        virtual void draw_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) override;

        virtual void fill_rect(const RolUI::Rect& rect) override;
        virtual void fill_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) override;

        virtual void fill_roundrect(const RolUI::Rect& rect, uint32_t round) override;
        virtual void fill_roundrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) override;

        virtual void fill_ellipse(const RolUI::Rect& rect) override;
        virtual void fill_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) override;
    };

} // namespace RolUIBackend
