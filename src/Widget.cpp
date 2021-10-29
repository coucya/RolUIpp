#include <stdexcept>

#include "RolUI/Widget.hpp"

namespace RolUI {

    Widget::Widget() noexcept
        : WidgetBase() {}

    Widget::Widget(Widget* parent) noexcept
        : WidgetBase(parent) {}

    Widget::~Widget() {}

    Point Widget::pos() { return _pos; }
    Size Widget::size() { return _size; }

    Widget* Widget::parent() noexcept {
        return static_cast<Widget*>(WidgetBase::parent());
    }
    const Widget* Widget::parent() const noexcept {
        return static_cast<const Widget*>(WidgetBase::parent());
    }

    Widget::ChlidrenView Widget::children_view(bool reverse) noexcept {
        return _children.prime_list().view<Widget>(&Widget::_brother, reverse);
    }
    const Widget::ChlidrenView Widget::children_view(bool reverse) const noexcept {
        return _children.prime_list().view<Widget>(&Widget::_brother, reverse);
    }

    void Widget::set_pos(const Point& pos) noexcept { _pos = pos; }
    void Widget::set_size(const Size& size) noexcept { _size = size; }

    void Widget::set_pos(int32_t x, int32_t y) noexcept { set_pos({x, y}); }
    void Widget::set_size(uint32_t w, uint32_t h) noexcept { set_size({w, h}); }

    void Widget::draw(IPainter* painter) {}

} // namespace RolUI
