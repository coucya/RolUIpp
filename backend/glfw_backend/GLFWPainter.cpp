#include <stdexcept>

#include "nanovg.h"

#include "RolUI/Color.h"
#include "GLFWPainter.h"

namespace RolUIBackend {

    static NVGcolor rc_to_nc(const RolUI::Color& c) {
        return nvgRGBA(c.r, c.g, c.b, c.a);
    }

    GLFWPainter::GLFWPainter(void* nvg_context) : _nvg_context(nvg_context) {}

    GLFWPainter::~GLFWPainter() {}

    void GLFWPainter::set_stroke_color(RolUI::Color color) { _stroke_color = color; }
    void GLFWPainter::set_fill_color(RolUI::Color color) { _fill_color = color; }

    void GLFWPainter::set_stroke_width(uint32_t w) { _stroke_width = w; }

    void GLFWPainter::draw_rect(const RolUI::Rect& rect) {
        draw_rect(rect.x, rect.y, rect.width, rect.height);
    }

    void GLFWPainter::draw_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, x, y, w, h);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_roundedrect(const RolUI::Rect& rect, uint32_t round) {
        draw_roundedrect(rect.x, rect.y, rect.width, rect.height, round);
    }
    void GLFWPainter::draw_roundedrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, round);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_ellipse(const RolUI::Rect& rect) {
        RolUI::Point centre = rect.centre();
        float rx = (float)rect.width / 2.0;
        float ry = (float)rect.height / 2.0;

        NVGcontext* vg = (NVGcontext*)_nvg_context;

        nvgBeginPath(vg);
        nvgEllipse(vg, centre.x, centre.y, rx, ry);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }
    void GLFWPainter::draw_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        draw_ellipse(RolUI::Rect(x, y, w, h));
    }

    void GLFWPainter::draw_circle(const RolUI::Point& centre, uint32_t r) {
        draw_circle(centre.x, centre.y, r);
    }
    void GLFWPainter::draw_circle(int32_t x, int32_t y, uint32_t r) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, x, y, r);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::fill_rect(const RolUI::Rect& rect) {
        fill_rect(rect.x, rect.y, rect.width, rect.height);
    }
    void GLFWPainter::fill_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, x, y, w, h);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_roundedrect(const RolUI::Rect& rect, uint32_t round) {
        fill_roundedrect(rect.x, rect.y, rect.width, rect.height, round);
    }
    void GLFWPainter::fill_roundedrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, round);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_ellipse(const RolUI::Rect& rect) {
        RolUI::Point centre = rect.centre();
        float rx = (float)rect.width / 2.0;
        float ry = (float)rect.height / 2.0;

        NVGcontext* vg = (NVGcontext*)_nvg_context;

        nvgBeginPath(vg);
        nvgEllipse(vg, centre.x, centre.y, rx, ry);
        nvgFillColor(vg, rc_to_nc(_stroke_color));
        nvgFill(vg);
    }
    void GLFWPainter::fill_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        fill_ellipse(RolUI::Rect(x, y, w, h));
    }

    void GLFWPainter::fill_circle(const RolUI::Point& centre, uint32_t r) {
        fill_circle(centre.x, centre.y, r);
    }
    void GLFWPainter::fill_circle(int32_t x, int32_t y, uint32_t r) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, x, y, r);
        nvgFillColor(vg, rc_to_nc(_stroke_color));
        nvgFill(vg);
    }
} // namespace RolUIBackend
