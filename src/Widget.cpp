
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <tuple>
#include <utility>

#include "RolUI/Application.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    bool send_event(Widget* w, IEvent* e) {
        if (!w) return false;
        e->_target = w;
        return w->handle_event(e);
    }

    Size perlayout(Widget* w, Constraint constraint) {
        return w->layout(constraint);
    }
    void set_rect(Widget* w, Rect rect) {
        if (!w) return;
        w->_pos = rect.pos();
        w->_size = rect.size();
    }

    Widget::Widget() noexcept {}

    Widget::~Widget() {}

    Point Widget::pos() const noexcept { return _pos; }
    Size Widget::size() const noexcept { return _size; }

    Rect Widget::rect() const noexcept { return {pos(), size()}; }

    Point Widget::abs_pos() const noexcept { return _abs_pos; }
    Rect Widget::abs_rect() const noexcept { return {abs_pos(), size()}; }

    Widget* Widget::parent() const noexcept { return _parent; }

    Widget* Widget::get_child_by_pos(Point pos) const noexcept { return nullptr; }

    bool Widget::handle_event(IEvent* e) noexcept { return false; }

    void Widget::draw(IPainter* painter) noexcept {}

    Size Widget::layout(Constraint constraint) noexcept { return {0, 0}; }

    bool Widget::hit_test(Point pos) const noexcept {
        return abs_rect().contain(pos);
    }

    void Widget::update_pos() noexcept {
        this->_abs_pos = parent() ? parent()->abs_pos() + pos() : pos();
    }

    SingleChildWidget::SingleChildWidget() noexcept {}

    Widget* SingleChildWidget::child() const noexcept { return _child; }
    void SingleChildWidget::set_child(Widget* child) noexcept {
        if (_child != nullptr)
            _child->_parent = nullptr;
        _child = child;
        _child->_parent = this;
    }

    Widget* SingleChildWidget::get_child_by_pos(Point pos) const noexcept {
        if (_child == nullptr) return nullptr;

        if (_child->hit_test(pos))
            return _child;
        else
            return nullptr;
    }

    void SingleChildWidget::draw(IPainter* painter) noexcept {
        if (_child == nullptr) return;

        RolUI::Rect ar = abs_rect();
        RolUI::Rect current_scissor = painter->get_scissor();
        painter->scissor(
            current_scissor
                .intersected(ar)
                .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
        // painter->set_base_pos(ar.pos() + _child->pos());
        _child->draw(painter);
        painter->scissor(current_scissor);
    }

    Size SingleChildWidget::layout(Constraint constraint) noexcept {
        if (_child == nullptr) return {0, 0};
        Size s = _child->layout(constraint);
        RolUI::set_rect(_child, RolUI::Rect{Point{0, 0}, s});
        return s;
    }

    void SingleChildWidget::update_pos() noexcept {
        Widget::update_pos();
        if (_child) _child->update_pos();
    }

    MultiChildWidget::MultiChildWidget() noexcept {}

    int MultiChildWidget::child_count() const noexcept { return _children.size(); }
    Widget* MultiChildWidget::child(int index) const noexcept {
        if (index < 0 || index >= _children.size()) return nullptr;
        return _children[index];
    }
    void MultiChildWidget::add_child(Widget* child) noexcept {
        child->_parent = this;
        _children.push_back(child);
    }
    void MultiChildWidget::set_child(int index, Widget* child) noexcept {
        if (index < 0 || index > _children.size()) return;
        if (index == _children.size()) {
            add_child(child);
            return;
        }
        _children[index]->_parent = nullptr;

        child->_parent = this;
        _children[index] = child;
    }

    void MultiChildWidget::remove_child(Widget* child) noexcept {
        _children.erase(std::find(_children.begin(), _children.end(), child));
    }
    void MultiChildWidget::remove_child(int index) noexcept {
        if (index < 0 || index >= _children.size()) return;
        _children.erase(_children.begin() + index);
    }

    Widget* MultiChildWidget::get_child_by_pos(Point pos) const noexcept {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
            if ((*it)->hit_test(pos))
                return *it;
        return nullptr;
    }

    void MultiChildWidget::draw(IPainter* painter) noexcept {
        if (_children.size() == 0) return;

        RolUI::Rect ar = abs_rect();
        RolUI::Rect current_scissor = painter->get_scissor();
        painter->scissor(
            current_scissor
                .intersected(ar)
                .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
        for (Widget* w : _children) {
            // painter->set_base_pos(ar.pos() + w->pos());
            w->draw(painter);
        }
        painter->scissor(current_scissor);
    }

    Size MultiChildWidget::layout(Constraint constraint) noexcept {
        if (_children.size() == 0) return {0, 0};

        Size res{0, 0};
        for (Widget* w : _children) {
            Size size = w->layout(constraint);
            RolUI::set_rect(w, {Point{0, 0}, size});
            res.width = std::max(res.width, size.width);
            res.height = std::max(res.height, size.height);
        }
        return res;
    }
    void MultiChildWidget::update_pos() noexcept {
        Widget::update_pos();
        for (Widget* w : _children)
            w->update_pos();
    }

} // namespace RolUI
