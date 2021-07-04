#include <stdexcept>

#include "RolUI/Widget.h"

namespace RolUI {

    WidgetListener::WidgetListener() noexcept
        : _in_use_list(&_ListenerNode::brother), _free_list(&_ListenerNode::brother) {
        for (int i = 0; i < WidgetListener::max_linstener_number; i++) {
            _ListenerNode* node = &_linsteners[i];
            _free_list.insert_back(node);
        }
    }
    WidgetListener::~WidgetListener() {}

    void WidgetListener::add_listener(EventType et, CallbackFun cb) {
        if (cb == nullptr) return;

        _ListenerNode* node = _free_list.front();
        if (node == nullptr) return;

        node->event_type = et;
        node->callback = cb;

        _free_list.remove(node);
        _in_use_list.append(node);
    }
    void WidgetListener::remove_listener(EventType et, CallbackFun cb) {
        if (cb == nullptr) return;

        for (auto& node : _in_use_list) {
            if (node.event_type == et && node.callback == cb) {
                node.clear();
                _in_use_list.remove(&node);
                _free_list.append(&node);
            }
        }
        return;
    }

    bool WidgetListener::on_event(IEvent* e) {
        bool res = false;

        for (auto& node : _in_use_list) {
            if (node.event_type == e->event_type())
                res = (node.callback ? node.callback(e) : false) || res;
        }
        return res;
    }

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

    Widget::ChlidrenView Widget::children_view_reverse() noexcept {
        return ChlidrenView(_children.rbegin(), _children.rend(), &Widget::_brother);
    }
    const Widget::ChlidrenView Widget::children_view_reverse() const noexcept {
        return ChlidrenView(_children.rbegin(), _children.rend(), &Widget::_brother);
    }

    Point Widget::pos() { return _pos; }
    Size Widget::size() { return _size; }

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
