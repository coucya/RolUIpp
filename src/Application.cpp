#include <cstdint>

#include "RolUI/IEvent.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/Widget_event.hpp"

namespace RolUI {

    static bool _should_exit = false;
    static Window* _window = nullptr;
    static Widget* _focus_widget = nullptr;
    static RootWidget _root_widget{};
    static double _current_time = 0.0;

    RootWidget::RootWidget() noexcept {}

    Widget* RootWidget::content_widget() const noexcept { return child(0); }
    void RootWidget::set_content_widget(Widget* widget) noexcept { set_child(widget, 0); }

    const ObjectType* RootWidget::object_type() const noexcept { return object_type_of<RootWidget>(); }

    RolUI_impl_object_type_of(RootWidget, Widget);

    Size RootWidget::perform_layout(Constraint constraint) noexcept {
        for (int i = 0; i < child_count(); i++) {
            Widget* w = child(i);
            Size size = w->layout(constraint);
            RolUI::set_pos(w, Point{0, 0});
        }
        return constraint.max();
    }

    void Application::init(Window* w) noexcept {
        _window = w;
        root_widget()->_mounted = true;
        root_widget()->_depth = 0;
        root_widget()->_parent = nullptr;
    }

    Window* Application::window() noexcept { return _window; }
    RootWidget* Application::root_widget() noexcept { return &_root_widget; }

    bool Application::has_focus_widget(Widget* w) noexcept { return _focus_widget != nullptr; }
    Widget* Application::focus_widget() noexcept { return _focus_widget; }
    void Application::set_focus_widget(Widget* w) noexcept {
        if (_focus_widget) {
            FocusChangeEvent fce{_focus_widget, false, true};
            send_event(_focus_widget, &fce);
        }

        _focus_widget = w;
        if (_focus_widget) {
            FocusChangeEvent fce{_focus_widget, true, false};
            send_event(_focus_widget, &fce);
        }
    }

    Widget* Application::get_widget_by_pos(Point pos) noexcept {
        Widget* widget = nullptr;
        Widget* w_it = root_widget();
        w_it->clear_hit();

        while ((widget = w_it->hit_test_children(pos)) != nullptr)
            w_it = widget;

        return w_it == root_widget() ? nullptr : w_it;
    }

    void Application::exit() noexcept { _should_exit = true; }
    bool Application::should_exit() noexcept { return _should_exit; }

    void Application::flush_frame(double time) noexcept {
        _current_time = time;
        flush_animation();
        flush_layout();
        flush_draw();
    }

    void Application::flush_animation() noexcept {}
    void Application::flush_layout() noexcept {
        Widget* root = root_widget();
        if (root) {
            root->layout({{0, 0}, _window->size()});
            RolUI::set_pos(root, {0, 0});
            root->update_pos();
        }
    }
    void Application::flush_draw() noexcept {
        if (!_window) return;
        Widget* root = root_widget();
        IPainter* painter = _window->painter();

        if (root == nullptr) return;
        if (painter == nullptr) return;

        _window->begin_draw();
        painter->clip_rect(Rect{Point(), _window->size()});
        root->draw(painter);
        _window->end_draw();
    }

} // namespace RolUI
