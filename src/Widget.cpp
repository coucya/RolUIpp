#include <stdexcept>

#include "RolUI/Widget.h"

namespace RolUI {

    Widget::Widget() noexcept
        : _parent(nullptr), _window(nullptr),
          _children(&Widget::_brother) {}

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

    Widget::ChlidrenView Widget::children_view(bool reverse) noexcept {
        return _children.view(reverse);
    }
    const Widget::ChlidrenView Widget::children_view(bool reverse) const noexcept {
        return _children.view(reverse);
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

        _children.insert_back(widget);
        widget->_parent = this;

        _set_window(widget->window());
        _set_window_for_chilren(widget->window());
    }
    void Widget::remove_child(Widget* widget) noexcept {
        if (widget == nullptr) return;
        if (widget->_parent != this) return;

        _children.remove(widget);
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
