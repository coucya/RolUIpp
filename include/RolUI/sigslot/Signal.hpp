#pragma once

#include <cstddef>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <vector>
#include <tuple>
#include <optional>

#include "ISignal.hpp"
#include "Slot.hpp"

namespace RolUI {

    class HasSlot;

    template <typename... Args>
    class Signal : public ISignal {
      private:
        typedef std::tuple<std::function<void(Args...)>, _details::SlotInfo> Slot;
        typedef std::vector<Slot> SlotList;

        typedef void (*FuncPtr)(Args...);

      public:
        Signal() {}
        ~Signal() override { disconnect_all(); }

        size_t size() const noexcept { return _slots.size(); }

        size_t connect(FuncPtr fp) noexcept;

        template <typename Type, typename Member,
                  typename = std::enable_if_t<std::is_invocable_r_v<void, Member, std::remove_reference_t<Type>*, Args...>>>
        size_t connect(Type&& obj, Member member) noexcept;

        template <typename Type, typename Member,
                  typename = std::enable_if_t<std::is_invocable_r_v<void, Member, Type*, Args...>>>
        size_t connect(Type* obj, Member member) noexcept;

        template <typename Type,
                  typename = std::enable_if_t<std::is_invocable_r_v<void, Type, Args...>>>
        size_t connect(Type&& callable) noexcept;

        template <typename Type,
                  typename = std::enable_if_t<std::is_invocable_r_v<void, Type, Args...>>>
        size_t connect(Type* callable) noexcept;

        void disconnect(size_t handle) noexcept;
        void disconnect(FuncPtr fp) noexcept;

        template <typename Type>
        void disconnect(Type* obj) noexcept;

        template <typename Type, typename Member,
                  typename = std::enable_if_t<std::is_invocable_r_v<void, Member, Type*, Args...>>>
        void disconnect(Type* obj, Member member) noexcept;

        void disconnect_all() noexcept;

        void emit(Args... args) const noexcept;

        void operator()(Args... args) const noexcept { emit(args...); }

      private:
        size_t _find(void* obj, _details::MemberType member) const noexcept;

        size_t _find(size_t handle) const noexcept;

        std::optional<size_t> _exist(void* obj, _details::MemberType member) const noexcept;

        size_t _connect(std::function<void(Args...)>&& cb,
                        void* obj = nullptr, _details::MemberType member = nullptr,
                        HasSlot* slot_ptr = nullptr) noexcept;

        void _disconnect_by_index(size_t idx) noexcept;

        void _remove_slot_by_handle(size_t handld) noexcept override;
        void _remove_slot_by_index(size_t idx) noexcept;

      private:
        SlotList _slots;
    };

    template <typename... Args>
    size_t Signal<Args...>::connect(FuncPtr fp) noexcept {
        void* obj = reinterpret_cast<void*>(fp);
        auto h_op = _exist(obj, nullptr);
        if (h_op.has_value()) return h_op.value();

        return _connect(fp, obj);
    }

    template <typename... Args>
    template <typename Type, typename Member, typename>
    size_t Signal<Args...>::connect(Type&& obj, Member member) noexcept {
        std::remove_reference_t<Type> tmp{std::forward<Type>(obj)};
        auto f = [=, obj = std::move(tmp)](Args... args) mutable {
            (obj.*member)(args...);
        };
        return _connect(f, nullptr, _details::to_member_type(member));
    }

    template <typename... Args>
    template <typename Type, typename Member, typename>
    size_t Signal<Args...>::connect(Type* obj, Member member) noexcept {
        auto h_op = _exist(obj, _details::to_member_type(member));
        if (h_op.has_value()) return h_op.value();

        auto f = [=](Args... args) mutable { (obj->*member)(args...); };
        HasSlot* hp = nullptr;
        if (std::is_base_of_v<HasSlot, Type>)
            hp = static_cast<HasSlot*>(obj);
        return _connect(f, obj, _details::to_member_type(member), hp);
    }

    template <typename... Args>
    template <typename Type, typename>
    size_t Signal<Args...>::connect(Type&& callable) noexcept {
        return _connect(std::forward<Type>(callable));
    }

    template <typename... Args>
    template <typename Type, typename>
    size_t Signal<Args...>::connect(Type* callable) noexcept {
        auto h_op = _exist(callable, nullptr);
        if (h_op.has_value()) return h_op.value();

        auto f = [=](Args... args) mutable { (*callable)(args...); };
        HasSlot* hp = nullptr;
        if (std::is_base_of_v<HasSlot, Type>)
            hp = static_cast<HasSlot*>(callable);
        return _connect(f, callable, nullptr, hp);
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(size_t handle) noexcept {
        int idx = _find(handle);
        _disconnect_by_index(idx);
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(FuncPtr fp) noexcept {
        int idx = _find(reinterpret_cast<void*>(fp), nullptr);
        _disconnect_by_index(idx);
    }

    template <typename... Args>
    template <typename Type>
    void Signal<Args...>::disconnect(Type* obj) noexcept {
        int idx = _find(obj, nullptr);
        _disconnect_by_index(idx);
    }

    template <typename... Args>
    template <typename Type, typename Member, typename>
    void Signal<Args...>::disconnect(Type* obj, Member member) noexcept {
        int idx = _find(obj, _details::to_member_type(member));
        _disconnect_by_index(idx);
    }

    template <typename... Args>
    void Signal<Args...>::disconnect_all() noexcept {
        for (int i = _slots.size(); i > 0; i--)
            _disconnect_by_index(i - 1);
    }

    template <typename... Args>
    void Signal<Args...>::emit(Args... args) const noexcept {
        for (auto& it : _slots) {
            auto& [cb, info] = it;
            if (cb) cb(args...);
        }
    }

    template <typename... Args>
    size_t Signal<Args...>::_find(void* obj, _details::MemberType member) const noexcept {
        auto it = std::find_if(_slots.begin(), _slots.end(), [=](const Slot& slot) {
            const auto& [cb, info] = slot;
            return info.obj_ptr == obj && info.member_ptr == member;
        });
        return std::distance(_slots.begin(), it);
    }

    template <typename... Args>
    size_t Signal<Args...>::_find(size_t handle) const noexcept {
        auto it = std::find_if(_slots.begin(), _slots.end(), [=](const Slot& slot) {
            const auto& [cb, info] = slot;
            return info.handle == handle;
        });
        return std::distance(_slots.begin(), it);
    }

    template <typename... Args>
    std::optional<size_t> Signal<Args...>::_exist(void* obj, _details::MemberType member) const noexcept {
        size_t idx = _find(obj, member);
        if (idx >= _slots.size()) return {};
        const auto& [cb, info] = _slots[idx];
        return info.handle;
    }

    template <typename... Args>
    size_t Signal<Args...>::_connect(std::function<void(Args...)>&& cb,
                                     void* obj, _details::MemberType member,
                                     HasSlot* slot_ptr) noexcept {
        using namespace _details;

        SlotInfo si = make_slot_info(_alloc_handle(), obj, member, slot_ptr);
        _slots.push_back({std::move(cb), si});

        if (slot_ptr != nullptr)
            slot_ptr->_add_signal(this, si);

        return si.handle;
    }

    template <typename... Args>
    void Signal<Args...>::_disconnect_by_index(size_t idx) noexcept {
        if (idx >= _slots.size()) return;
        auto& [cb, si] = _slots[idx];

        HasSlot* hs = si.slot_ptr;
        if (hs) hs->_remove_signal(this, si.handle);

        _remove_slot_by_index(idx);
    }

    template <typename... Args>
    void Signal<Args...>::_remove_slot_by_handle(size_t handld) noexcept {
        size_t idx = _find(handld);
        _remove_slot_by_index(idx);
    }

    template <typename... Args>
    void Signal<Args...>::_remove_slot_by_index(size_t idx) noexcept {
        if (idx >= _slots.size()) return;
        _slots.erase(_slots.begin() + idx);
    }

} // namespace RolUI
