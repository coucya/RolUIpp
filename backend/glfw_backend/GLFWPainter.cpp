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

    void GLFWPainter::draw_line(const RolUI::Point& a, const RolUI::Point& b) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgMoveTo(vg, a.x, b.x);
        nvgLineTo(vg, b.x, b.y);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_rect(const RolUI::Rect& rect) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, rect.x, rect.y, rect.width, rect.height);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::draw_roundedrect(const RolUI::Rect& rect, uint32_t round) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, rect.x, rect.y, rect.width, rect.height, round);
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

    void GLFWPainter::draw_circle(const RolUI::Point& centre, uint32_t r) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, centre.x, centre.y, r);
        nvgStrokeColor(vg, rc_to_nc(_stroke_color));
        nvgStrokeWidth(vg, _stroke_width);
        nvgStroke(vg);
    }

    void GLFWPainter::fill_rect(const RolUI::Rect& rect) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRect(vg, rect.x, rect.y, rect.width, rect.height);
        nvgFillColor(vg, rc_to_nc(_fill_color));
        nvgFill(vg);
    }

    void GLFWPainter::fill_roundedrect(const RolUI::Rect& rect, uint32_t round) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgRoundedRect(vg, rect.x, rect.y, rect.width, rect.height, round);
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

    void GLFWPainter::fill_circle(const RolUI::Point& centre, uint32_t r) {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        nvgBeginPath(vg);
        nvgCircle(vg, centre.x, centre.y, r);
        nvgFillColor(vg, rc_to_nc(_stroke_color));
        nvgFill(vg);
    }

} // namespace RolUIBackend
