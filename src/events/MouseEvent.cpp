#include <stdexcept>

#include "RolUI/events/MouseEvent.h"

namespace RolUI {

    bool MouseDispatcher::is_key_change(MouseKey key) const noexcept {
        return _key_is_change[(int)key - 1];
    }

    void MouseDispatcher::set_pos(Point pos) noexcept {
        if (_pos_is_change) {
            _current_pos = pos;
        } else {
            _last_pos = _current_pos;
            _current_pos = pos;
        }
        _pos_is_change = true;
    }
    void MouseDispatcher::set_pos(int32_t x, int32_t y) noexcept {
        set_pos({x, y});
    }

    void MouseDispatcher::set_last_pos(Point pos) noexcept { _last_pos = pos; }
    void MouseDispatcher::set_last_pos(int32_t x, int32_t y) noexcept {
        set_last_pos({x, y});
    }

    void MouseDispatcher::set_key_mode(MouseKey key, MouseKeyMode mode) noexcept {
        int idx = (int)key - 1;
        if (_key_mode[idx] != mode)
            _key_is_change[idx] = true;
        _key_mode[idx] = mode;
    }

    void MouseDispatcher::clear_change() noexcept {
        _pos_is_change = false;
        for (int i = 0; i < sizeof(_key_is_change); i++) {
            _key_is_change[i] = false;
        }
    }

    void MouseDispatcher::distribute(Widget* root_widget) {
        if (root_widget == nullptr) return;

        _distribute_to_widget(root_widget, root_widget->pos());

        clear_change();
    }

    bool MouseDispatcher::_distribute_to_widget(Widget* w, Point widget_pos) {

        auto chilren = w->children_view_reverse();

        bool is_do = false;
        for (auto& c : chilren) {
            Point c_widget_pos = widget_pos + c.pos();
            is_do = _distribute_to_widget(&c, c_widget_pos);
            if (is_do) break;
        }

        Point mouse_pos = _current_pos - widget_pos;
        Rect widget_rect = Rect{Point(), w->size()};

        if (!is_do && widget_rect.is_contain_point(mouse_pos)) {
            MouseEvent event{_last_pos, _current_pos};
            event.set_widget_pos(widget_pos);
            is_do = w->do_event(&event);
        }

        return is_do;
    }

    MouseEvent::MouseEvent(Point last_pos, Point current_pos) noexcept
        : _last_pos(last_pos), _current_pos(current_pos) {}

    MouseEvent::~MouseEvent() {}

    Point MouseEvent::pos() const noexcept {
        return _current_pos - _widget_pos;
    }
    Vector MouseEvent::offset() const noexcept {
        return _current_pos - _last_pos;
    }
    MouseKey MouseEvent::button() const noexcept {
        return MouseKey::key1;
    }

    void MouseEvent::set_widget_pos(Point p) { _widget_pos = p; }
} // namespace RolUI
