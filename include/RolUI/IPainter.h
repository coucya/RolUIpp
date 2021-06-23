#pragma once

#include <stdint.h>

#include "Rect.h"

namespace RolUI {

    class IPainter {
      public:
        virtual ~IPainter() {}

        virtual void draw_rect(const Rect& rect) = 0;
        virtual void draw_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void draw_roundrect(const Rect& rect, uint32_t round) = 0;
        virtual void draw_roundrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) = 0;

        virtual void draw_ellipse(const Rect& rect) = 0;
        virtual void draw_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void fill_rect(const Rect& rect) = 0;
        virtual void fill_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void fill_roundrect(const Rect& rect, uint32_t round) = 0;
        virtual void fill_roundrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) = 0;

        virtual void fill_ellipse(const Rect& rect) = 0;
        virtual void fill_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;
    };

} // namespace RolUI
