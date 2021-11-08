
#include "RolUI/Window.hpp"
#include "RolUI/widgets/LabelWidget.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    LabelWidget::LabelWidget() noexcept
        : _font_size(15), _font_color(Color()) {

        add_listener(WindowChangeEvent::type(), [this](IEvent* e) {
            this->_update_size();
            return true;
        });
    }

    LabelWidget::~LabelWidget() {}

    void LabelWidget::_update_size() {
        Window* win = window();

        if (win && !_text.empty()) {
            Size s = win->painter()->text_size(_text.c_str(), _text.size());
            Widget::set_size(s);
        }
    }

    void LabelWidget::set_font(const char* name) noexcept {
        _font_name = name;
    }
    void LabelWidget::set_font_size(uint32_t size) noexcept {
        _font_size = size;
    }
    void LabelWidget::set_font_color(Color c) noexcept {
        _font_color = c;
    }

    void LabelWidget::set_text(const char* text) {
        if (text == nullptr) return;
        set_text(text, strlen(text));
    }
    void LabelWidget::set_text(const char* text, uint32_t len) noexcept {
        if (text == nullptr) return;
        _text = std::string(text, len);
        _update_size();
    }

    void LabelWidget::on_draw(IPainter* painter) {
        painter->set_font_size(_font_size);
        painter->set_font_color(_font_color);
        painter->set_font(_font_name);
        painter->draw_text(pos(), _text.c_str(), _text.size());
    }

} // namespace RolUI
