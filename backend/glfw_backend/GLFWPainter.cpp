#include <iostream>
#include <cstdint>
#include <stdint.h>

#include "nanovg.h"

#include "GLFWPainter.h"

#include "RolUI/Color.hpp"
#include "RolUI/Point.hpp"

namespace RolUIBackend {

    static NVGcolor rc_to_nc(const RolUI::Color& c) {
        return nvgRGBA(c.r, c.g, c.b, c.a);
    }

    GLFWPainter::GLFWPainter(void* nvg_context) : _nvg_context(nvg_context) {}

    GLFWPainter::~GLFWPainter() {}

    RolUI::Size GLFWPainter::text_size(const char* text, uint32_t len) const {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        float bound[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        nvgFontFaceId(vg, _font_id);
        nvgFontSize(vg, _font_size);
        nvgTextBounds(vg, 0, 0, text, text + len, bound);
        return {(int32_t)(bound[2] - bound[0]),
                (int32_t)(bound[3] - bound[1])};
    }
    uint32_t GLFWPainter::text_glyph_pos(const char* text, uint32_t text_len,
                                         uint32_t* out_min_x, uint32_t* out_max_x, uint32_t pos_len) const {
        constexpr int T_MAX_LEN = 1024;
        NVGglyphPosition T_GLYPH_POSITION[T_MAX_LEN];

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        NVGglyphPosition* glyphs = &T_GLYPH_POSITION[0];
        if (pos_len > T_MAX_LEN)
            glyphs = new NVGglyphPosition[pos_len];

        int n = nvgTextGlyphPositions(vg, 0, 0, text, text + text_len, glyphs, pos_len);
        for (int i = 0; i < n; i++) {
            out_min_x[i] = glyphs[i].minx;
            out_max_x[i] = glyphs[i].maxx;
        }

        if (pos_len > T_MAX_LEN) delete[] glyphs;

        return n;
    }

    int32_t GLFWPainter::create_image(const uint8_t* data, uint32_t w, uint32_t h) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        return nvgCreateImageRGBA(vg, w, h, 0, data);
    }
    RolUI::Size GLFWPainter::image_size(int32_t handle) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        int w, h;
        nvgImageSize(vg, handle, &w, &h);
        return {w, h};
    }
    void GLFWPainter::delete_image(int32_t handle) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgDeleteImage(vg, handle);
    }

    int32_t GLFWPainter::create_font(const uint8_t* data, uint32_t len) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        return nvgCreateFontMem(vg, "", (uint8_t*)data, len, 0);
    }
    void GLFWPainter::set_font(int32_t handle) {
        if (handle >= 0) _font_id = handle;
    }
    void GLFWPainter::set_font_size(uint32_t s) { _font_size = s; }
    void GLFWPainter::set_font_color(RolUI::Color color) { _font_color = color; }

    // void GLFWPainter::set_base_pos(RolUI::Point pos) { _pos = pos; }
    void GLFWPainter::clip_rect(RolUI::Rect rect) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgIntersectScissor(vg, rect.x, rect.y, rect.width, rect.height);
        auto nr = _clip_rect.intersected(rect);
        _clip_rect = nr.has_value() ? nr.value() : Rect{rect.pos(), Size{0, 0}};
    }
    RolUI::Rect GLFWPainter::get_clip_rect() const {
        return _clip_rect;
    }
    void GLFWPainter::save_clip() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgSave(vg);
    }
    void GLFWPainter::restore_clip() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgRestore(vg);
    }

    void GLFWPainter::set_fill_color(RolUI::Color color) { _fill_color = color; }

    void GLFWPainter::set_line_color(RolUI::Color color) { _line_color = color; }
    void GLFWPainter::set_line_width(uint32_t w) { _line_width = w; }
    void GLFWPainter::set_line_cap(int32_t cap) { _line_cap = cap; }
    void GLFWPainter::set_line_join(int32_t join) { _line_join = join; }

    void GLFWPainter::fill_text(RolUI::Point pos, const char* text, uint32_t len) {
        if (!text || len == 0) return;

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgFontFaceId(vg, _font_id);
        nvgFontSize(vg, _font_size);
        nvgFillColor(vg, rc_to_nc(_font_color));
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(vg, pos.x, pos.y, text, text + len);
    }

    void GLFWPainter::draw_image(RolUI::Point pos, RolUI::Size size, int handle) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        NVGpaint p = nvgImagePattern(vg, pos.x, pos.y, size.width, size.height, 0, handle, 1.0f);

        nvgBeginPath(vg);
        nvgRect(vg, pos.x, pos.y, size.width, size.height);
        nvgClosePath(vg);
        nvgFillPaint(vg, p);
        nvgFill(vg);
    }

    void GLFWPainter::draw_line(RolUI::Point pa, RolUI::Point pb) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgMoveTo(vg, pa.x, pa.y);
        nvgLineTo(vg, pb.x, pb.y);
        nvgStrokeColor(vg, rc_to_nc(_line_color));
        nvgStrokeWidth(vg, _line_width);
        nvgStroke(vg);
    }

    void GLFWPainter::stroke_rect(RolUI::Rect rect) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, rect.x, rect.y, rect.width, rect.height);
        nvgStrokeColor(vg, rc_to_nc(_line_color));
        nvgStrokeWidth(vg, _line_width);
        nvgStroke(vg);
    }

    void GLFWPainter::stroke_roundedrect(RolUI::Rect rect, uint32_t round) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, rect.x, rect.y, rect.width, rect.height, round);
        nvgStrokeColor(vg, rc_to_nc(_line_color));
        nvgStrokeWidth(vg, _line_width);
        nvgStroke(vg);
    }

    void GLFWPainter::stroke_ellipse(RolUI::Rect rect) {
        RolUI::Point centre = rect.centre_middle();
        float rx = (float)rect.width / 2.0;
        float ry = (float)rect.height / 2.0;

        NVGcontext* vg = (NVGcontext*)_nvg_context;

        nvgBeginPath(vg);
        nvgEllipse(vg, centre.x, centre.y, rx, ry);
        nvgStrokeColor(vg, rc_to_nc(_line_color));
        nvgStrokeWidth(vg, _line_width);
        nvgStroke(vg);
    }

    void GLFWPainter::stroke_circle(RolUI::Point centre, uint32_t r) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, centre.x, centre.y, r);
        nvgStrokeColor(vg, rc_to_nc(_line_color));
        nvgStrokeWidth(vg, _line_width);
        nvgStroke(vg);
    }

    void GLFWPainter::fill_rect(RolUI::Rect rect) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, rect.x, rect.y, rect.width, rect.height);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_roundedrect(RolUI::Rect rect, uint32_t round) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, rect.x, rect.y, rect.width, rect.height, round);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_ellipse(RolUI::Rect rect) {
        RolUI::Point centre = rect.centre_middle();
        float rx = (float)rect.width / 2.0;
        float ry = (float)rect.height / 2.0;

        NVGcontext* vg = (NVGcontext*)_nvg_context;

        nvgBeginPath(vg);
        nvgEllipse(vg, centre.x, centre.y, rx, ry);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_circle(RolUI::Point centre, uint32_t r) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, centre.x, centre.y, r);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

} // namespace RolUIBackend
