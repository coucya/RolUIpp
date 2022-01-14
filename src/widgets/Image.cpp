
#include "RolUI/Application.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/Image.hpp"

namespace RolUI {
    namespace widget {

        Image::Image(RolUI::Image image, Fit fit) noexcept {
            this->image = image;
            this->fit = fit;
        }

        Image::~Image() {}

        void Image::on_draw(IPainter* painter) {
            Fit fit = this->fit.get();
            int img_handle = image->handle();
            Size img_size = Application::window()->painter()->image_size(this->image.get().handle());
            float w_rate = float(size().width) / float(size().height);
            float img_rate = float(img_size.width) / float(img_size.height);

            if (fit == Fit::fill) {
                painter->draw_image({0, 0}, size(), img_handle);
            } else if (fit == Fit::cover) {
                if (w_rate == img_rate) {
                    painter->draw_image({0, 0}, img_size, img_handle);
                } else if (w_rate > img_rate) {
                    int w = size().width;
                    int h = float(w) / float(img_size.width) * img_size.height;
                    int y = -((h - float(size().height)) / 2.0f);
                    painter->draw_image({0, y}, {w, h}, img_handle);
                } else if (w_rate < img_rate) {
                    int h = size().height;
                    int w = float(h) / float(img_size.height) * img_size.width;
                    int x = -((w - float(size().width)) / 2.0f);
                    painter->draw_image({x, 0}, {w, h}, img_handle);
                }
            } else if (fit == Fit::contain) {
                if (w_rate == img_rate) {
                    painter->draw_image({0, 0}, img_size, img_handle);
                } else if (w_rate > img_rate) {
                    int h = size().height;
                    int w = float(h) / float(img_size.height) * img_size.width;
                    int x = float(size().width - w) / 2.0f;
                    painter->draw_image({x, 0}, {w, h}, img_handle);
                } else if (w_rate < img_rate) {
                    int w = size().width;
                    int h = float(w) / float(img_size.width) * img_size.height;
                    int y = float(size().height - h) / 2.0f;
                    painter->draw_image({0, y}, {w, h}, img_handle);
                }
            }
        }
        Size Image::perlayout(Constraint constraint) {
            return constraint.max();
        }
    } // namespace widget
} // namespace RolUI
