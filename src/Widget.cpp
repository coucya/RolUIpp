
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
        if (!w || !e) return false;
        if (!w->mounted()) return false;
        e->_target = w;
        return w->handle_event(e);
    }

    void set_pos(Widget* w, Point pos) {
        if (!w) return;
        w->_pos = pos;
    }

    Widget::Widget() noexcept {}

    Widget::~Widget() {}

    Point Widget::pos() const noexcept { return _pos; }
    Size Widget::size() const noexcept { return _size; }

    Rect Widget::rect() const noexcept { return {pos(), size()}; }

    Point Widget::abs_pos() const noexcept { return _abs_pos; }
    Rect Widget::abs_rect() const noexcept { return {abs_pos(), size()}; }

    bool Widget::mounted() const noexcept { return _mounted; }
    unsigned Widget::depth() const noexcept { return _depth; }
    Widget* Widget::parent() const noexcept { return _parent; }

    Size Widget::layout(Constraint constraint) noexcept {
        _size = perform_layout(constraint);
        return _size;
    }

    Widget* Widget::get_child_by_pos(Point pos) const noexcept { return nullptr; }

    void Widget::visit_children(std::function<void(Widget*)> f) noexcept {}

    bool Widget::hit_test(Point pos) const noexcept {
        return abs_rect().contain(pos);
    }

    bool Widget::handle_event(IEvent* e) noexcept { return false; }

    void Widget::draw(IPainter* painter) noexcept {}

    Size Widget::perform_layout(Constraint constraint) noexcept { return {0, 0}; }

    void Widget::update_pos() noexcept {
        this->_abs_pos = parent() ? parent()->abs_pos() + pos() : pos();
    }

    void Widget::_mount(Widget* parent) noexcept {
        if (_parent || !parent) return;
        _parent = parent;
        _mounted = parent->_mounted;
        if (parent->_mounted)
            _attach();
    }
    void Widget::_unmount() noexcept {
        if (!_parent) return;
        if (_mounted)
            _detach();
        _parent = nullptr;
        _mounted = false;
    }

    void Widget::_attach() noexcept {
        _mounted = true;
        _depth = _parent ? _parent->_depth + 1 : 0;
        visit_children([](Widget* child) {
            child->_attach();
        });
    }
    void Widget::_detach() noexcept {
        _mounted = false;
        _depth = 0;
        visit_children([](Widget* child) {
            child->_detach();
        });
    }

    SingleChildWidget::SingleChildWidget() noexcept {}

    Widget* SingleChildWidget::child() const noexcept { return _child; }
    SingleChildWidget* SingleChildWidget::set_child(Widget* child) noexcept {
        if (_child)
            _child->_unmount();
        _child = child;
        if (_child)
            _child->_mount(this);
        return this;
    }
    void SingleChildWidget::remove_child() noexcept {
        if (_child)
            _child->_unmount();
        _child = nullptr;
    }

    Widget* SingleChildWidget::get_child_by_pos(Point pos) const noexcept {
        if (_child == nullptr) return nullptr;

        if (_child->hit_test(pos))
            return _child;
        else
            return nullptr;
    }
    void SingleChildWidget::visit_children(std::function<void(Widget*)> f) noexcept {
        if (_child) f(_child);
    }

    void SingleChildWidget::draw(IPainter* painter) noexcept {
        if (_child == nullptr) return;

        RolUI::Rect ar = abs_rect();
        RolUI::Rect current_scissor = painter->get_scissor();
        painter->scissor(
            current_scissor
                .intersected(ar)
                .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
        _child->draw(painter);
        painter->scissor(current_scissor);
    }

    Size SingleChildWidget::perform_layout(Constraint constraint) noexcept {
        Size child_size = layout_child(constraint, [](Size) { return Point{0, 0}; });
        return child_size;
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
    MultiChildWidget* MultiChildWidget::add_child(Widget* child) noexcept {
        if (!child) return this;
        _children.push_back(child);
        child->_mount(this);
        return this;
    }
    MultiChildWidget* MultiChildWidget::set_child(int index, Widget* child) noexcept {
        if (!child || index < 0 || index > _children.size()) return this;

        if (index == _children.size()) {
            add_child(child);
        } else {
            _children[index]->_unmount();
            _children[index] = child;
            _children[index]->_mount(this);
        }
        return this;
    }

    void MultiChildWidget::remove_child(Widget* child) noexcept {
        auto it = std::find(_children.begin(), _children.end(), child);
        if (it != _children.end())
            (*it)->_unmount();
        _children.erase(it);
    }
    void MultiChildWidget::remove_child(int index) noexcept {
        if (index < 0 || index >= _children.size()) return;
        _children[index]->_unmount();
        _children.erase(_children.begin() + index);
    }

    Widget* MultiChildWidget::get_child_by_pos(Point pos) const noexcept {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
            if ((*it)->hit_test(pos))
                return *it;
        return nullptr;
    }
    void MultiChildWidget::visit_children(std::function<void(Widget*)> f) noexcept {
        for (Widget* w : _children) f(w);
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
            w->draw(painter);
        }
        painter->scissor(current_scissor);
    }

    Size MultiChildWidget::perform_layout(Constraint constraint) noexcept {
        if (_children.size() == 0) return {0, 0};

        Size self_size{0, 0};
        for (Widget* w : _children) {
            Size size = w->layout(constraint);
            RolUI::set_pos(w, Point{0, 0});
            self_size.width = std::max(self_size.width, size.width);
            self_size.height = std::max(self_size.height, size.height);
        }
        return self_size;
    }
    void MultiChildWidget::update_pos() noexcept {
        Widget::update_pos();
        for (Widget* w : _children)
            w->update_pos();
    }

} // namespace RolUI
