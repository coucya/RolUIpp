
#include <climits>

#include "RolUI/Window.hpp"

namespace RolUI {

    Window::Window() noexcept {}
    Window::~Window() {}

    IPainter* Window::painter() { return nullptr; }
    void Window::begin_draw() {}
    void Window::end_draw() {}

    void Window::dispatch_event(double timeout) {}

} // namespace RolUI
