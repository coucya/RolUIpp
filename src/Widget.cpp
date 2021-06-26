#include <stdexcept>

#include "RolUI/Widget.h"

namespace RolUI {

    Widget::Widget(Widget* parent) noexcept
        : Widget() {
        set_parent(parent);
    }

    Widget::~Widget() {
        if (_parent != nullptr)
            _parent->remove_child(this);
        _parent = nullptr;
        _window = nullptr;
    }

    Widget::ChlidrenView Widget::children_view() noexcept {
        return ChlidrenView(_children.begin(), _children.end(), &Widget::_brother);
    }
    const Widget::ChlidrenView Widget::children_view() const noexcept {
        return ChlidrenView(_children.begin(), _children.end(), &Widget::_brother);
    }

    Point Widget::pos() const { return _pos; }
    Size Widget::size() const { return _size; }

    void Widget::set_pos(const Point& pos) noexcept { _pos = pos; }
    void Widget::set_size(const Size& size) noexcept { _size = size; }

    void Widget::set_pos(int32_t x, int32_t y) noexcept { set_pos({x, y}); }
    void Widget::set_size(uint32_t w, uint32_t h) noexcept { set_size({w, h}); }

    void Widget::set_parent(Widget* widget) noexcept {
        if (widget == nullptr) return;
        widget->add_child(this);
    }

    void Widget::add_child(Widget* widget) noexcept {
        if (widget == nullptr) return;
        if (widget->_parent != nullptr)
            widget->_parent->remove_child(widget);

        _children.insert_back(&widget->_brother);
        widget->_parent = this;

        _set_window(widget->window());
        _set_window_for_chilren(widget->window());
    }
    void Widget::remove_child(Widget* widget) noexcept {
        if (widget == nullptr) return;
        if (widget->_parent != this) return;

        _children.remove(&widget->_brother);
        widget->_parent = nullptr;

        _set_window(nullptr);
        _set_window_for_chilren(nullptr);
    }

    void Widget::add_listener(WidgetEventListener* listener) noexcept {
        if (listener == nullptr) return;

        _listeners.insert_back(&listener->_brother);
    }
    void Widget::remove_Listener(WidgetEventListener* listener) noexcept {
        if (listener == nullptr) return;

        _listeners.remove(&listener->_brother);
    }

    bool Widget::event_distribute_to_children(IEvent* event) {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it) {
            auto cw = contain_of(&*it, &Widget::_brother);
            if (cw->do_event(event)) return true;
        }
        return false;
    }
    bool Widget::event_distribute_to_listener(IEvent* event) {
        bool res = false;
        for (auto it = _listeners.begin(); it != _listeners.end(); ++it) {
            auto wel = contain_of(&*it, &WidgetEventListener::_brother);
            res = res || wel->do_event(event);
        };
        return res;
    }

    bool Widget::on_event(IEvent* event) {
        if (event_distribute_to_children(event))
            return true;
        return event_distribute_to_listener(event);
    }

    void Widget::draw(IPainter* painter) {}

    void Widget::_set_window(Window* w) noexcept {
        _window = w;
    }
    void Widget::_set_window_for_chilren(Window* w) {
        auto chilren = children_view();
        for (auto& c : chilren) {
            c._set_window(w);
            c._set_window_for_chilren(w);
        }
    }

} // namespace RolUI
