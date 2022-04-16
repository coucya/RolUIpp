
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <tuple>
#include <utility>

#include "RolUI/IEvent.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Object.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

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

    bool Widget::is_hit() const noexcept { return _is_hit; }
    bool Widget::mounted() const noexcept { return _mounted; }
    unsigned Widget::depth() const noexcept { return _depth; }
    Widget* Widget::parent() const noexcept { return _parent; }

    void Widget::mark_hit() noexcept { _is_hit = true; }
    void Widget::clear_hit_self() noexcept { _is_hit = false; }
    void Widget::clear_hit() noexcept {
        clear_hit_self();
        visit_child(this, [](Widget* child) { child->clear_hit(); });
    }

    Size Widget::layout(Constraint constraint) noexcept {
        _size = perform_layout(constraint);
        return _size;
    }

    int Widget::child_count() const noexcept { return 0; }
    Widget* Widget::child(int index) const noexcept { return nullptr; }
    Widget* Widget::set_child(Widget* child, int index) noexcept { return this; }
    void Widget::remove_child(int index) noexcept {}

    bool Widget::hit_test(Point pos) noexcept {
        bool result = hit_test_self(pos);
        for (int i = 0; i < child_count(); i++)
            result = result || child(i)->hit_test(pos);
        return result;
    }
    bool Widget::hit_test_self(Point pos) noexcept {
        return _is_hit = abs_rect().contain(pos);
    }
    Widget* Widget::hit_test_children(Point pos) noexcept {
        Widget* result = nullptr;
        for (int i = 0; i < child_count(); i++)
            result = child(i)->hit_test_self(pos) ? child(i) : result;
        return result;
    }

    bool Widget::handle_event(IEvent* e) noexcept { return false; }

    void Widget::draw(IPainter* painter) noexcept {}

    Size Widget::perform_layout(Constraint constraint) noexcept { return {0, 0}; }

    void Widget::update_pos() noexcept {
        this->_abs_pos = parent() ? parent()->abs_pos() + pos() : pos();
        visit_child(this, [](Widget* child) {
            child->update_pos();
        });
    }

    void Widget::_mount(Widget* parent) noexcept {
        if (!parent) return;
        if (_parent) _unmount();
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
        visit_child(this, [](Widget* child) {
            child->_attach();
        });
    }
    void Widget::_detach() noexcept {
        visit_child(this, [](Widget* child) {
            child->_detach();
        });
        _mounted = false;
        _depth = 0;
    }

    SingleChildWidget::SingleChildWidget() noexcept {}

    int SingleChildWidget::child_count() const noexcept { return _child ? 1 : 0; }
    Widget* SingleChildWidget::child(int index) const noexcept { return index == 0 && _child ? _child : nullptr; }
    Widget* SingleChildWidget::set_child(Widget* child, int index) noexcept {
        if (index != 0) return this;
        if (_child)
            _child->_unmount();
        _child = child;
        if (_child)
            _child->_mount(this);
        return this;
    }
    void SingleChildWidget::remove_child(int index) noexcept {
        if (index != 0) return;
        if (_child)
            _child->_unmount();
        _child = nullptr;
    }

    void SingleChildWidget::draw(IPainter* painter) noexcept {
        if (_child)
            _child->draw(painter);
    }

    Size SingleChildWidget::perform_layout(Constraint constraint) noexcept {
        Size child_size = layout_child(constraint, [](Size) { return Point{0, 0}; });
        return child_size;
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
    Widget* MultiChildWidget::set_child(Widget* child, int index) noexcept {
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
    MultiChildWidget* MultiChildWidget::insert_child(int index, Widget* child) noexcept {
        if (!child || index < 0 || index > _children.size()) return this;
        if (index == _children.size()) {
            add_child(child);
        } else {
            _children.insert(_children.begin() + index, child);
            _children[index]->_mount(this);
        }
        return this;
    }

    void MultiChildWidget::remove_child(Widget* child) noexcept {
        auto it = std::find(_children.begin(), _children.end(), child);
        remove_child(it - _children.begin());
    }
    void MultiChildWidget::remove_child(int index) noexcept {
        if (index < 0 || index >= _children.size()) return;
        _children[index]->_unmount();
        _children.erase(_children.begin() + index);
    }
    void MultiChildWidget::remove_child_all() noexcept {
        for (int i = 0; i < _children.size(); i++)
            _children[i]->_unmount();
        _children.clear();
    }

    void MultiChildWidget::draw(IPainter* painter) noexcept {
        for (Widget* w : _children)
            w->draw(painter);
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

    const ObjectType* Widget::object_type() const noexcept { return object_type_of<Widget>(); }
    const ObjectType* SingleChildWidget::object_type() const noexcept { return object_type_of<SingleChildWidget>(); }
    const ObjectType* MultiChildWidget::object_type() const noexcept { return object_type_of<MultiChildWidget>(); }

    RolUI_impl_object_type_of(Widget, Object, HasSlot);
    RolUI_impl_object_type_of(SingleChildWidget, Widget);
    RolUI_impl_object_type_of(MultiChildWidget, Widget);

} // namespace RolUI
