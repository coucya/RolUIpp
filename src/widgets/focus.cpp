
#include "RolUI/widgets/focus.hpp"

namespace RolUI {
    namespace widget {

        Focus::Focus() noexcept {}

        void Focus::focus() noexcept {
            FocusManager::set_focus_widget(this);
        }
        void Focus::unfocus() noexcept {
            if (FocusManager::focus_widget() == this)
                FocusManager::set_focus_widget(nullptr);
        }

    } // namespace widget

    widget::Focus* FocusManager::_focus_widget = nullptr;

    bool FocusManager::has_focus_widget() noexcept { return _focus_widget != nullptr; }
    widget::Focus* FocusManager::focus_widget() noexcept { return _focus_widget; }
    void FocusManager::set_focus_widget(widget::Focus* widget) noexcept {
        if (_focus_widget != nullptr)
            _focus_widget->on_focus.emit(false);

        _focus_widget = widget;
        _focus_widget->on_focus.emit(true);
    }

} // namespace RolUI