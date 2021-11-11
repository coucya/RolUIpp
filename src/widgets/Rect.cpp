
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"

namespace RolUI {
    namespace widget {

        Rect::Rect(int x, int y, unsigned w, unsigned h, unsigned round) noexcept {
            set_pos(x, y);
            set_size((int)w, (int)h);
            set_round(round);
        }
        Rect::~Rect() {}

        unsigned Rect::round() const noexcept { return _propertys.round; }
        unsigned Rect::border_width() const noexcept { return _propertys.border_width; }

        Color Rect::border_color() const noexcept { return _propertys.border_color; }
        Color Rect::background_color() const noexcept { return _propertys.background_color; }

        const Rect::StyleProperty& Rect::style_property() const noexcept { return _propertys; }

        void Rect::set_round(unsigned r) noexcept {
            _propertys.round = r;
        }

        void Rect::set_border_width(unsigned w) noexcept {
            _propertys.border_width = w;
        }

        void Rect::set_border_color(const Color& c) noexcept {
            _propertys.border_color = c;
        }
        void Rect::set_background_color(const Color& c) noexcept {
            _propertys.background_color = c;
        }

        void Rect::set_style_property(const StyleProperty& property) noexcept {
            _propertys.round = property.round;
            _propertys.border_color = property.border_color;
            _propertys.border_width = property.border_width;
            _propertys.background_color = property.background_color;
        }

        void Rect::on_draw(IPainter* painter) {
            RolUI::Rect rect_ = rect();
            painter->set_fill_color(_propertys.background_color);
            painter->fill_roundedrect(rect_, _propertys.round);

            if (_propertys.border_width > 0) {
                painter->set_stroke_color(_propertys.border_color);
                painter->set_stroke_width(_propertys.border_width * 2);
                painter->draw_roundedrect(rect_, _propertys.round);
            }
        }

    } // namespace widget
} // namespace RolUI
