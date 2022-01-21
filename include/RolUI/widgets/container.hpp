#pragma once

#include <vector>

#include "../Widget.hpp"
#include "../IPainter.hpp"
#include "../Property.hpp"

namespace RolUI {
    namespace widget {

        class SingleChildWidget : public Widget {
          private:
            Widget* _child = nullptr;

          public:
            SingleChildWidget() noexcept {}

            Widget* child() const noexcept { return _child; }
            void set_child(Widget* child) {
                if (_child != nullptr)
                    _child->_parent = nullptr;
                _child = child;
                _child->_parent = this;
            }

            virtual Widget* get_child_by_pos(Point pos) const noexcept override {
                if (_child == nullptr) return nullptr;

                if (_child->hit_test(pos - _child->pos()))
                    return _child;
                else
                    return nullptr;
            }

            virtual void on_draw(IPainter* painter) override {
                if (_child == nullptr) return;

                RolUI::Rect ar = abs_rect();
                RolUI::Rect current_scissor = painter->get_scissor();
                painter->scissor(
                    current_scissor
                        .intersected(ar)
                        .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
                painter->set_base_pos(ar.pos() + _child->pos());
                _child->on_draw(painter);
                painter->scissor(current_scissor);
            }

            virtual Size perlayout(Constraint constraint) override {
                if (_child == nullptr) return {0, 0};
                Size s = _child->perlayout(constraint);
                RolUI::set_rect(_child, RolUI::Rect{Point{0, 0}, s});
                return s;
            }
        };

        class MultiChildWidget : public Widget {
          private:
            typedef std::vector<Widget*> Children;
            Children _children;

          public:
            MultiChildWidget() noexcept {}

            int child_count() const noexcept { return _children.size(); }
            Widget* child(int index) const noexcept {
                if (index < 0 || index >= _children.size()) return nullptr;
                return _children[index];
            }
            void add_child(Widget* child) noexcept {
                child->_parent = this;
                _children.push_back(child);
            }
            void set_child(int index, Widget* child) noexcept {
                if (index < 0 || index > _children.size()) return;
                if (index == _children.size()) {
                    add_child(child);
                    return;
                }
                _children[index]->_parent = nullptr;

                child->_parent = this;
                _children[index] = child;
            }

            Widget* at(int index) const noexcept {
                return _children.empty() ? nullptr : _children.at(index);
            }

            Widget*& operator[](int index) noexcept { return _children[index]; }
            Widget* const& operator[](int index) const noexcept { return _children[index]; }

            virtual Widget* get_child_by_pos(Point pos) const noexcept override {
                for (Widget* w : _children)
                    if (w->hit_test(pos - w->pos()))
                        return w;
                return nullptr;
            }

            virtual void on_draw(IPainter* painter) override {
                if (_children.size() == 0) return;

                RolUI::Rect ar = abs_rect();
                RolUI::Rect current_scissor = painter->get_scissor();
                painter->scissor(
                    current_scissor
                        .intersected(ar)
                        .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
                for (Widget* w : _children) {
                    painter->set_base_pos(ar.pos() + w->pos());
                    w->on_draw(painter);
                }
                painter->scissor(current_scissor);
            }

            virtual Size perlayout(Constraint constraint) override {
                if (_children.size() == 0) return {0, 0};

                Size res{0, 0};
                for (Widget* w : _children) {
                    Size size = w->perlayout(constraint);
                    RolUI::set_rect(w, {Point{0, 0}, size});
                    res.width = std::max(res.width, size.width);
                    res.height = std::max(res.height, size.height);
                }
                return res;
            }
        };

    } // namespace widget
} // namespace RolUI