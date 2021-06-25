#include <stdexcept>

#include "RolUI/Window.h"
#include "RolUI/Widget.h"
#include "RolUI/IPainter.h"

namespace RolUI {

    IPainter* Window::painter() {
        throw std::runtime_error("RolUI::Window::painter(): not impl.");
    }

    void Window::set_widget(Widget* widget) {
        if (widget == nullptr) return;

        if (_widget != nullptr) {
            _widget->_set_window(nullptr);
            _widget->_set_window_for_chilren(nullptr);
        }

        _widget = widget;

        _widget->_set_window(this);
        _widget->_set_window_for_chilren(this);
    }

    void Window::_draw_widget(RolUI::Widget* widget, RolUI::IPainter* painter) {
        if (widget == nullptr) return;

        widget->draw(painter);

        auto children = widget->children_view();
        for (auto& c : children) {
            _draw_widget(&c, painter);
        }
    }
    void Window::draw() {
        Widget* root_widget = this->_widget;
        IPainter* painter = this->painter();

        if (root_widget == nullptr) return;
        if (painter == nullptr) return;

        _draw_widget(root_widget, painter);
    }

} // namespace RolUI
