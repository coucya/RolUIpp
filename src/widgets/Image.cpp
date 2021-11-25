
#include "RolUI/IEvent.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/Image.hpp"

namespace RolUI {
    namespace widget {

        Image::Image(Widget* parent) noexcept
            : Widget(parent), _image_handle(0) {}
        Image::Image(Widget* parent, int image_handle) noexcept
            : Widget(parent), _image_handle(image_handle) {}

        Image::~Image() {}

        int Image::image() const noexcept { return _image_handle; }
        void Image::set_image(int img_handle) noexcept { _image_handle = img_handle; }

        void Image::adjust_size() noexcept {
            Window* win = window();
            if (win) {
                Size s = win->painter()->image_size(_image_handle);
                Widget::set_size(s);
            }
        };

        void Image::on_draw(IPainter* painter) {
            painter->draw_image({0, 0}, size(), _image_handle);
        }

    } // namespace widget
} // namespace RolUI
