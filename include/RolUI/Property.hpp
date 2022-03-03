#pragma once

// #include <optional>
#include <type_traits>

#include "./sigslot/Signal.hpp"
#include "./sigslot/Slot.hpp"

namespace RolUI {

    class Widget;

    template <typename T>
    using enable_if_constructble_t = typename std::enable_if<std::is_constructible<T>::value>::type;
    template <typename T>
    using enable_if_copy_constructble_t = typename std::enable_if<std::is_copy_constructible<T>::value>::type;
    template <typename T>
    using enable_if_move_constructble_t = typename std::enable_if<std::is_move_constructible<T>::value>::type;
    template <typename T>
    using enable_if_copy_assignable_t = typename std::enable_if<std::is_copy_assignable<T>::value>::type;
    template <typename T>
    using enable_if_move_assignable_t = typename std::enable_if<std::is_move_assignable<T>::value>::type;

    template <typename WT, typename T>
    class Property : public HasSlot {
      public:
        Signal<const T&> on_change;

      public:
        template <typename = enable_if_constructble_t<T>>
        explicit Property(WT* this_) noexcept;

        template <typename = enable_if_copy_constructble_t<T>>
        Property(WT* this_, const T& v) noexcept;
        template <typename = enable_if_move_constructble_t<T>>
        Property(WT* this_, T&& v) noexcept;

        Property(const Property& val) = delete;
        Property(Property&& val) = delete;

        ~Property() = default;

        Property& operator=(const Property& right) = delete;
        Property& operator=(Property&& right) = delete;

        template <typename = enable_if_copy_assignable_t<T>>
        Property& operator=(const T& v) noexcept;
        template <typename = enable_if_move_assignable_t<T>>
        Property& operator=(T&& v) noexcept;

        // Property& operator=(const std::optional<T>& v) noexcept;
        // Property& operator=(std::optional<T>&& v) noexcept;

        template <typename = enable_if_copy_assignable_t<T>>
        WT* operator()(const T& v) noexcept;
        template <typename = enable_if_move_assignable_t<T>>
        WT* operator()(T&& v) noexcept;

        T& operator()() noexcept;
        const T& operator()() const noexcept;

        T* operator->() noexcept;
        const T* operator->() const noexcept;

        operator T const&() const;

        T& get() noexcept;
        const T& get() const noexcept;

        template <typename = enable_if_copy_assignable_t<T>>
        WT* set(const T& v) noexcept;
        template <typename = enable_if_move_assignable_t<T>>
        WT* set(T&& v) noexcept;

        // void set(const std::optional<T>& v) noexcept;
        // void set(std::optional<T>&& v) noexcept;

        template <typename = enable_if_copy_assignable_t<T>>
        WT* set_no_notify(const T& v) noexcept;
        template <typename = enable_if_move_assignable_t<T>>
        WT* set_no_notify(T&& v) noexcept;

        // void set_no_notify(const std::optional<T>& v) noexcept;
        // void set_no_notify(std::optional<T>&& v) noexcept;

      private:
        WT* _this = nullptr;
        T _data;
    };

    template <typename WT, typename T>
    template <typename>
    Property<WT, T>::Property(WT* this_) noexcept
        : _this(this_), _data() {}

    template <typename WT, typename T>
    template <typename>
    Property<WT, T>::Property(WT* this_, const T& v) noexcept
        : _this(this_), _data(v) {}
    template <typename WT, typename T>
    template <typename>
    Property<WT, T>::Property(WT* this_, T&& v) noexcept
        : _this(this_), _data(std::move(v)) {}

    template <typename WT, typename T>
    template <typename>
    Property<WT, T>& Property<WT, T>::operator=(const T& v) noexcept {
        set(v);
        return *this;
    }
    template <typename WT, typename T>
    template <typename>
    Property<WT, T>& Property<WT, T>::operator=(T&& v) noexcept {
        set(std::move(v));
        return *this;
    }

    // template <typename WT, typename T>
    // Property<T,WT>& Property<T,WT>::operator=(const std::optional<T>& v) noexcept {
    //     set(v);
    //     return *this;
    // }
    // template <typename WT, typename T>
    // Property<T,WT>& Property<T,WT>::operator=(std::optional<T>&& v) noexcept {
    //     set(std::move(v));
    //     return *this;
    // }

    template <typename WT, typename T>
    template <typename>
    WT* Property<WT, T>::operator()(const T& v) noexcept { return set(v); }
    template <typename WT, typename T>
    template <typename>
    WT* Property<WT, T>::operator()(T&& v) noexcept { return set(std::move(v)); }

    template <typename WT, typename T>
    T& Property<WT, T>::operator()() noexcept { return _data; }
    template <typename WT, typename T>
    const T& Property<WT, T>::operator()() const noexcept { return _data; }

    template <typename WT, typename T>
    T* Property<WT, T>::operator->() noexcept { return &_data; }
    template <typename WT, typename T>
    const T* Property<WT, T>::operator->() const noexcept { return &_data; }

    template <typename WT, typename T>
    Property<WT, T>::operator T const&() const { return get(); }

    template <typename WT, typename T>
    T& Property<WT, T>::get() noexcept { return _data; }
    template <typename WT, typename T>
    const T& Property<WT, T>::get() const noexcept { return _data; }

    template <typename WT, typename T>
    template <typename>
    WT* Property<WT, T>::set(const T& v) noexcept {
        _data = v;
        on_change.emit(_data);
        return _this;
    }
    template <typename WT, typename T>
    template <typename>
    WT* Property<WT, T>::set(T&& v) noexcept {
        _data = std::move(v);
        on_change.emit(_data);
        return _this;
    }

    // template <typename WT, typename T>
    // void Property<T,WT>::set(const std::optional<T>& v) noexcept {
    //     if (v.has_value()) set(v.value());
    // }
    // template <typename WT, typename T>
    // void Property<T,WT>::set(std::optional<T>&& v) noexcept {
    //     if (v.has_value()) set(std::move(v.value()));
    // }

    template <typename WT, typename T>
    template <typename>
    WT* Property<WT, T>::set_no_notify(const T& v) noexcept {
        _data = v;
        return _this;
    }
    template <typename WT, typename T>
    template <typename>
    WT* Property<WT, T>::set_no_notify(T&& v) noexcept {
        _data = std::move(v);
        return _this;
    }

    // template <typename WT, typename T>
    // void Property<T,WT>::set_no_notify(const std::optional<T>& v) noexcept {
    //     if (v.has_value()) set_no_notify(v.value());
    // }
    // template <typename WT, typename T>
    // void Property<T,WT>::set_no_notify(std::optional<T>&& v) noexcept {
    //     if (v.has_value()) set_no_notify(std::move(v.value()));
    // }

} // namespace RolUI
