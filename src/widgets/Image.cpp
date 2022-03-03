
#include "RolUI/Application.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/Image.hpp"

namespace RolUI {
    namespace widgets {

        ImageWidget::ImageWidget() noexcept {}
        ImageWidget::ImageWidget(RolUI::Image image, Fit fit) noexcept {
            this->image = image;
            this->fit = fit;
        }

        ImageWidget::~ImageWidget() {}

        void ImageWidget::draw(IPainter* painter) noexcept {
            Fit fit = this->fit.get();
            int img_handle = image->handle();
            Size img_size = painter->image_size(this->image.get().handle());
            float widget_rate = float(size().width) / float(size().height);
            float img_rate = float(img_size.width) / float(img_size.height);

            if (fit == Fit::fill) {
                painter->draw_image(abs_pos(), size(), img_handle);
            } else if (fit == Fit::cover) {
                if (widget_rate == img_rate) {
                    painter->draw_image(abs_pos(), img_size, img_handle);
                } else if (widget_rate > img_rate) {
                    int w = size().width;
                    int h = float(w) / float(img_size.width) * img_size.height;
                    int y = -((h - float(size().height)) / 2.0f);
                    painter->draw_image({abs_pos().x, abs_pos().y + y}, {w, h}, img_handle);
                } else if (widget_rate < img_rate) {
                    int h = size().height;
                    int w = float(h) / float(img_size.height) * img_size.width;
                    int x = -((w - float(size().width)) / 2.0f);
                    painter->draw_image({abs_pos().x + x, abs_pos().y}, {w, h}, img_handle);
                }
            } else if (fit == Fit::contain) {
                if (widget_rate == img_rate) {
                    painter->draw_image(abs_pos(), img_size, img_handle);
                } else if (widget_rate > img_rate) {
                    int h = size().height;
                    int w = float(h) / float(img_size.height) * img_size.width;
                    int x = float(size().width - w) / 2.0f;
                    painter->draw_image({abs_pos().x + x, abs_pos().y}, {w, h}, img_handle);
                } else if (widget_rate < img_rate) {
                    int w = size().width;
                    int h = float(w) / float(img_size.width) * img_size.height;
                    int y = float(size().height - h) / 2.0f;
                    painter->draw_image({abs_pos().x, abs_pos().y + y}, {w, h}, img_handle);
                }
            }
        }
        Size ImageWidget::perform_layout(Constraint constraint) noexcept {
            return constraint.max();
        }
    } // namespace widgets
} // namespace RolUI
