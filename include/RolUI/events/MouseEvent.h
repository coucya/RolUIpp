#pragma once

#include <stdint.h>

#include "RolUI/Point.h"
#include "RolUI/vector.h"
#include "RolUI/IEvent.h"
#include "RolUI/Widget.h"

namespace RolUI {

    enum class MouseKeyMode {
        press,
        release,
    };

    enum class MouseKey {
        key1 = 1,
        key2,
        key3,
        key4,
        key5,
        key6,
        key7,
        key8,

        enum_count = 8,

        left = key1,
        right = key2,
        middle = key3,
    };

    class MouseEvent : public IEvent {
      public:
        MouseEvent(Point last_pos, Point current_pos) noexcept;
        ~MouseEvent() override;

        Point pos() const noexcept;
        Vector offset() const noexcept;
        MouseKey button() const noexcept;

        void set_widget_pos(Point p);

      private:
        Point _last_pos;
        Point _current_pos;
        Point _widget_pos;
    };

    class MouseDispatcher {
      public:
        Point pos() const noexcept { return _current_pos; }
        Vector offset() const noexcept { return _current_pos - _last_pos; }

        bool is_pos_change() const noexcept { return _pos_is_change; }
        bool is_move() const noexcept { return _pos_is_change; }

        bool is_key_change(MouseKey key) const noexcept;

        void set_pos(Point pos) noexcept;
        void set_pos(int32_t x, int32_t y) noexcept;

        void set_last_pos(Point pos) noexcept;
        void set_last_pos(int32_t x, int32_t y) noexcept;

        void set_key_mode(MouseKey key, MouseKeyMode mode) noexcept;

        void clear_change() noexcept;

        void distribute(Widget* root_widget);

      private:
        bool _distribute_to_widget(Widget* w, Point widget_pos);

      private:
        bool _pos_is_change;
        Point _last_pos;
        Point _current_pos;

        bool _key_is_change[(int)MouseKey::enum_count];
        MouseKeyMode _key_mode[(int)MouseKey::enum_count];
    };

} // namespace RolUI
