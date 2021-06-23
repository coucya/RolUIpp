#include <stdexcept>

#include "GLFWPainter.h"

namespace RolUIBackend {
    GLFWPainter::GLFWPainter() {}

    GLFWPainter::~GLFWPainter() {}

    void GLFWPainter::draw_rect(const RolUI::Rect& rect) {
        throw std::runtime_error("not impl.");
    }
    void GLFWPainter::draw_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        throw std::runtime_error("not impl.");
    }

    void GLFWPainter::draw_roundrect(const RolUI::Rect& rect, uint32_t round) {
        throw std::runtime_error("not impl.");
    }
    void GLFWPainter::draw_roundrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) {
        throw std::runtime_error("not impl.");
    }

    void GLFWPainter::draw_ellipse(const RolUI::Rect& rect) {
        throw std::runtime_error("not impl.");
    }
    void GLFWPainter::draw_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        throw std::runtime_error("not impl.");
    }

    void GLFWPainter::fill_rect(const RolUI::Rect& rect) {
        throw std::runtime_error("not impl.");
    }
    void GLFWPainter::fill_rect(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        throw std::runtime_error("not impl.");
    }

    void GLFWPainter::fill_roundrect(const RolUI::Rect& rect, uint32_t round) {
        throw std::runtime_error("not impl.");
    }
    void GLFWPainter::fill_roundrect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t round) {
        throw std::runtime_error("not impl.");
    }

    void GLFWPainter::fill_ellipse(const RolUI::Rect& rect) {
        throw std::runtime_error("not impl.");
    }
    void GLFWPainter::fill_ellipse(int32_t x, int32_t y, uint32_t w, uint32_t h) {
        throw std::runtime_error("not impl.");
    }
} // namespace RolUIBackend
