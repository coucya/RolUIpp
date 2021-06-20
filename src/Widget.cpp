#include <stdexcept>

#include "Widget.h"

namespace RolUI {
    Widget::Widget(Widget* parent) noexcept {
        set_parent(parent);
    }

    Widget::~Widget() {
    }

    void Widget::set_parent(Widget* widget) noexcept {
        if (widget == nullptr) return;
        widget->add_child(this);
    }

    void Widget::add_child(Widget* widget) {
        if (widget == nullptr) return;
        if (widget->_parent != nullptr)
            widget->_parent->remove_child(widget);

        _children.insert_back(&widget->_brother);
        widget->_parent = this;
    }
    void Widget::remove_child(Widget* widget) {
        if (widget == nullptr) return;
        if (widget->_parent != this) return;

        _children.remove(&widget->_brother);
        widget->_parent = nullptr;
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
} // namespace RolUI
