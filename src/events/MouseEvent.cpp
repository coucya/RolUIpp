#include <stdexcept>

#include "RolUI/events/MouseEvent.h"

namespace RolUI {
    MouseEvent::MouseEvent(Point last_pos, Point current_pos) noexcept
        : _last_pos(last_pos), _current_pos(current_pos) {}

    MouseEvent::~MouseEvent() {}

    Point MouseEvent::pos() const noexcept {
        return _current_pos - _widget_pos;
    }
    Vector MouseEvent::offset() const noexcept {
        return _current_pos - _last_pos;
    }
    MouseEvent::MouseKey MouseEvent::button() const noexcept {
        return MouseKey_1;
    }

    void MouseEvent::set_widget_pos(Point p) { _widget_pos = p; }
} // namespace RolUI
