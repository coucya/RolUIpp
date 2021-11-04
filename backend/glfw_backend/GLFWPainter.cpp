
#include "nanovg.h"

#include "RolUI/Color.hpp"
#include "GLFWPainter.h"

namespace RolUIBackend {

    static NVGcolor rc_to_nc(const RolUI::Color& c) {
        return nvgRGBA(c.r, c.g, c.b, c.a);
    }

    GLFWPainter::GLFWPainter(void* nvg_context) : _nvg_context(nvg_context) {}

    GLFWPainter::~GLFWPainter() {}

    bool GLFWPainter::load_font(const char* name, const char* filename) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        return nvgCreateFont(vg, name, filename) >= 0;
    }

    RolUI::Size GLFWPainter::text_size(const char* text, uint32_t len) const {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        float bound[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        nvgTextBounds(vg, 0, 0, text, text + len, bound);
        return {(uint32_t)(bound[2] - bound[0]),
                (uint32_t)(bound[3] - bound[1])};
    }

    void GLFWPainter::set_base_pos(const RolUI::Point& pos) { _pos = pos; }
    void GLFWPainter::scissor(const RolUI::Rect& rect) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgScissor(vg, rect.x, rect.y, rect.width, rect.height);
    }

    void GLFWPainter::set_font_size(uint32_t s) { _font_size = s; }
    void GLFWPainter::set_font_color(RolUI::Color color) { _font_color = color; }
    void GLFWPainter::set_font(const char* name) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgFontFace(vg, name);
    }

    void GLFWPainter::set_stroke_color(RolUI::Color color) { _stroke_color = color; }
    void GLFWPainter::set_fill_color(RolUI::Color color) { _fill_color = color; }

    void GLFWPainter::set_stroke_width(uint32_t w) { _stroke_width = w; }

    void GLFWPainter::draw_text(const RolUI::Point& pos, const char* text, uint32_t len) {
        if (!text || len == 0) return;

        RolUI::Point p = _pos + pos;

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgFontSize(vg, _font_size);
        nvgFillColor(vg, rc_to_nc(_font_color));
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(vg, p.x, p.y, text, text + len);
    }

    void GLFWPainter::draw_line(const RolUI::Point& a, const RolUI::Point& b) {
        RolUI::Point pa = _pos + a;
        RolUI::Point pb = _pos + b;

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgMoveTo(vg, pa.x, pb.x);
        nvgLineTo(vg, pb.x, pb.y);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_rect(const RolUI::Rect& rect) {
        RolUI::Point p = _pos + rect.pos();

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, p.x, p.y, rect.width, rect.height);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_roundedrect(const RolUI::Rect& rect, uint32_t round) {
        RolUI::Point p = _pos + rect.pos();

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, p.x, p.y, rect.width, rect.height, round);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_ellipse(const RolUI::Rect& rect) {
        RolUI::Point centre = rect.centre_middle();
        float rx = (float)rect.width / 2.0;
        float ry = (float)rect.height / 2.0;

        RolUI::Point pc = _pos + centre;

        NVGcontext* vg = (NVGcontext*)_nvg_context;

        nvgBeginPath(vg);
        nvgEllipse(vg, pc.x, pc.y, rx, ry);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_circle(const RolUI::Point& centre, uint32_t r) {
        RolUI::Point pc = _pos + centre;

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, pc.x, pc.y, r);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::fill_rect(const RolUI::Rect& rect) {
        RolUI::Point p = _pos + rect.pos();

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, p.x, p.y, rect.width, rect.height);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_roundedrect(const RolUI::Rect& rect, uint32_t round) {
        RolUI::Point p = _pos + rect.pos();

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, p.x, p.y, rect.width, rect.height, round);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_ellipse(const RolUI::Rect& rect) {
        RolUI::Point centre = rect.centre_middle();
        float rx = (float)rect.width / 2.0;
        float ry = (float)rect.height / 2.0;

        RolUI::Point pc = _pos + centre;

        NVGcontext* vg = (NVGcontext*)_nvg_context;

        nvgBeginPath(vg);
        nvgEllipse(vg, pc.x, pc.y, rx, ry);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_circle(const RolUI::Point& centre, uint32_t r) {
        RolUI::Point pc = _pos + centre;

        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, pc.x, pc.y, r);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

} // namespace RolUIBackend
