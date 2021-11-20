#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <tuple>

#include "RolUI/WidgetState.hpp"

namespace RolUI {

    template <typename T>
    class Property {
      public:
        constexpr static WidgetState default_priority[] = {
            WIDGET_STATE_DISABLED,
            WIDGET_STATE_FOCUS,
            WIDGET_STATE_HOVER,
            WIDGET_STATE_PRESS,
            WIDGET_STATE_USER_STATE_1,
            WIDGET_STATE_USER_STATE_2,
            WIDGET_STATE_USER_STATE_3,
            WIDGET_STATE_DEFAULT,
        };

      public:
        Property() noexcept;

        Property(const Property& val) = default;
        Property(Property&& val) = default;

        ~Property() = default;

        Property& operator=(const Property& right) = default;
        Property& operator=(Property&& right) = default;

        Property& operator=(const T& v) noexcept;
        Property& operator=(T&& v) noexcept;
        Property& operator=(std::tuple<T, WidgetState>&& v) noexcept;

        operator T const&() const;

        void set(const T& v, WidgetState s = WIDGET_STATE_DEFAULT) noexcept;
        const T& get() const noexcept;
        const T& get(WidgetState state) const noexcept;
        const T& get(WidgetState state, const WidgetState* priority, unsigned len) const noexcept;
        const T& get(WidgetState state, std::initializer_list<WidgetState> priority) const noexcept;

      private:
        std::optional<T> _data[WIDGET_STATE_COUNT];
    };

    template <typename T>
    Property<T>::Property() noexcept {
        _data[0] = T{};
    }

    template <typename T>
    Property<T>& Property<T>::operator=(const T& v) noexcept {
        _data[0] = v;
        return *this;
    }
    template <typename T>
    Property<T>& Property<T>::operator=(T&& v) noexcept {
        _data[0] = std::move(v);
        return *this;
    }
    template <typename T>
    Property<T>& Property<T>::operator=(std::tuple<T, WidgetState>&& v) noexcept {
        set(std::get<0>(std::move(v)), std::get<1>(std::move(v)));
        return *this;
    }

    template <typename T>
    Property<T>::operator T const&() const { return get(); }

    template <typename T>
    void Property<T>::set(const T& v, WidgetState s) noexcept {
        widget_state_foreach(s, [&, this](WidgetState s, unsigned i) {
            this->_data[i] = v;
        });
    }

    template <typename T>
    const T& Property<T>::get() const noexcept {
        return _data[0].value();
    }
    template <typename T>
    const T& Property<T>::get(WidgetState state, const WidgetState* priority, unsigned len) const noexcept {
        for (unsigned i = 0; i < len; i++) {
            WidgetState s = priority[i];
            unsigned idx = widget_state_to_index(s);
            if (widget_state_has_state(state, s) && _data[idx].has_value())
                return _data[idx].value();
        }
        return _data[0].value();
    }
    template <typename T>
    const T& Property<T>::get(WidgetState state) const noexcept {
        return get(state, default_priority, sizeof(default_priority) / sizeof(WidgetState));
    }
    template <typename T>
    const T& Property<T>::get(WidgetState state, std::initializer_list<WidgetState> priority) const noexcept {
        return get(state, priority.begin(), priority.size());
    }

} // namespace RolUI
