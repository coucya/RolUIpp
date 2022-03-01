#pragma once

#include <optional>

#include "./sigslot/Signal.hpp"
#include "./sigslot/Slot.hpp"

namespace RolUI {

    class Widget;

    template <typename T>
    class Property : public HasSlot {

      public:
        Signal<const T&> on_change;

      public:
        explicit Property(Widget* this_) noexcept;

        Property(Widget* this_, const T& v) noexcept;
        Property(Widget* this_, T&& v) noexcept;

        Property(const Property& val) = delete;
        Property(Property&& val) = delete;

        ~Property() = default;

        Property& operator=(const Property& right) = delete;
        Property& operator=(Property&& right) = delete;

        Property& operator=(const T& v) noexcept;
        Property& operator=(T&& v) noexcept;

        // Property& operator=(const std::optional<T>& v) noexcept;
        // Property& operator=(std::optional<T>&& v) noexcept;

        T& operator()(const T& v) noexcept;
        T& operator()(T&& v) noexcept;

        T& operator()() noexcept;
        const T& operator()() const noexcept;

        T* operator->() noexcept;
        const T* operator->() const noexcept;

        operator T const&() const;

        T& get() noexcept;
        const T& get() const noexcept;

        void set(const T& v) noexcept;
        void set(T&& v) noexcept;

        void set(const std::optional<T>& v) noexcept;
        void set(std::optional<T>&& v) noexcept;

        void set_no_notify(const T& v) noexcept;
        void set_no_notify(T&& v) noexcept;

        void set_no_notify(const std::optional<T>& v) noexcept;
        void set_no_notify(std::optional<T>&& v) noexcept;

      private:
        Widget* _this = nullptr;
        T _data;
    };

    template <typename T>
    Property<T>::Property(Widget* this_) noexcept
        : _this(this_), _data() {}

    template <typename T>
    Property<T>::Property(Widget* this_, const T& v) noexcept
        : _this(this_), _data(v) {}
    template <typename T>
    Property<T>::Property(Widget* this_, T&& v) noexcept
        : _this(this_), _data(std::move(v)) {}

    template <typename T>
    Property<T>& Property<T>::operator=(const T& v) noexcept {
        set(v);
        return *this;
    }
    template <typename T>
    Property<T>& Property<T>::operator=(T&& v) noexcept {
        set(std::move(v));
        return *this;
    }

    // template <typename T>
    // Property<T>& Property<T>::operator=(const std::optional<T>& v) noexcept {
    //     set(v);
    //     return *this;
    // }
    // template <typename T>
    // Property<T>& Property<T>::operator=(std::optional<T>&& v) noexcept {
    //     set(std::move(v));
    //     return *this;
    // }

    template <typename T>
    T& Property<T>::operator()(const T& v) noexcept {
        set(v);
        return _data;
    }
    template <typename T>
    T& Property<T>::operator()(T&& v) noexcept {
        set(std::move(v));
        return _data;
    }

    template <typename T>
    T& Property<T>::operator()() noexcept { return _data; }
    template <typename T>
    const T& Property<T>::operator()() const noexcept { return _data; }

    template <typename T>
    T* Property<T>::operator->() noexcept { return &_data; }
    template <typename T>
    const T* Property<T>::operator->() const noexcept { return &_data; }

    template <typename T>
    Property<T>::operator T const&() const { return get(); }

    template <typename T>
    T& Property<T>::get() noexcept { return _data; }
    template <typename T>
    const T& Property<T>::get() const noexcept { return _data; }

    template <typename T>
    void Property<T>::set(const T& v) noexcept {
        _data = v;
        on_change.emit(_data);
    }
    template <typename T>
    void Property<T>::set(T&& v) noexcept {
        _data = std::move(v);
        on_change.emit(_data);
    }

    template <typename T>
    void Property<T>::set(const std::optional<T>& v) noexcept {
        if (v.has_value()) set(v.value());
    }
    template <typename T>
    void Property<T>::set(std::optional<T>&& v) noexcept {
        if (v.has_value()) set(std::move(v.value()));
    }

    template <typename T>
    void Property<T>::set_no_notify(const T& v) noexcept { _data = v; }
    template <typename T>
    void Property<T>::set_no_notify(T&& v) noexcept { _data = std::move(v); }

    template <typename T>
    void Property<T>::set_no_notify(const std::optional<T>& v) noexcept {
        if (v.has_value()) set_no_notify(v.value());
    }
    template <typename T>
    void Property<T>::set_no_notify(std::optional<T>&& v) noexcept {
        if (v.has_value()) set_no_notify(std::move(v.value()));
    }

} // namespace RolUI
