#include <stdexcept>

#include "RolUI/WidgetBase.h"

namespace RolUI {

    WidgetBase::WidgetBase() noexcept
        : _parent(nullptr), _window(nullptr),
          _children(&WidgetBase::_brother) {}

    WidgetBase::WidgetBase(WidgetBase* parent) noexcept
        : WidgetBase() {
        set_parent(parent);
    }

    WidgetBase::~WidgetBase() {
        if (_parent != nullptr)
            _parent->remove_child(this);
        _parent = nullptr;
        _window = nullptr;
    }

    void WidgetBase::set_parent(WidgetBase* widget) noexcept {
        if (widget == nullptr) return;
        widget->add_child(this);
    }

    void WidgetBase::add_child(WidgetBase* widget) noexcept {
        if (widget == nullptr) return;
        if (widget->_parent != nullptr)
            widget->_parent->remove_child(widget);

        _children.insert_back(widget);
        widget->_parent = this;

        _set_window(widget->window());
        _set_window_for_chilren(widget->window());
    }
    void WidgetBase::remove_child(WidgetBase* widget) noexcept {
        if (widget == nullptr) return;
        if (widget->_parent != this) return;

        _children.remove(widget);
        widget->_parent = nullptr;

        _set_window(nullptr);
        _set_window_for_chilren(nullptr);
    }

    void WidgetBase::_set_window(Window* w) noexcept {
        _window = w;
    }
    void WidgetBase::_set_window_for_chilren(Window* w) {
        auto chilren = _children.view();
        for (auto& c : chilren) {
            c._set_window(w);
            c._set_window_for_chilren(w);
        }
    }

} // namespace RolUI
