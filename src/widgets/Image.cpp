
#include "RolUI/Application.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/Image.hpp"

namespace RolUI {
    namespace widget {

        Image::Image(Widget* parent) noexcept
            : Widget(parent) {}
        Image::Image(Widget* parent, RolUI::Image image) noexcept
            : Widget(parent) {
            this->image = image;
        }

        Image::~Image() {}

        void Image::adjust_size() noexcept {
            Size s = Application::window()->painter()->image_size(image.get().handle());
            Widget::set_size(s);
        };

        void Image::on_draw(IPainter* painter) {
            painter->draw_image({0, 0}, size(), image.get().handle());
        }

    } // namespace widget
} // namespace RolUI
