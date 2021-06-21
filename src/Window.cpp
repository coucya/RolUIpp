#include <stdexcept>

#include "RolUI/Window.h"
#include "RolUI/Widget.h"
#include "RolUI/IPainter.h"

namespace RolUI {

    IPainter* Window::painter() {
        throw std::runtime_error("RolUI::Window::painter(): not impl.");
    }

    void Window::draw() {
        Widget* root_widget = this->_widget;
        IPainter* painter = this->painter();

        if (root_widget == nullptr) return;
        if (painter == nullptr) return;

        auto rtw_children = root_widget->children_view();
        for (auto& cw : rtw_children) {
            cw.draw(painter);
        }
    }

} // namespace RolUI
