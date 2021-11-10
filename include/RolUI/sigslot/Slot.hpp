#pragma once

#include <vector>

#include "ISignal.hpp"

namespace RolUI {
    class HasSlot {

        template <typename... Args>
        friend class Signal;

      private:
        typedef std::tuple<ISignal*, _details::SlotInfo> SignalConnect;

      public:
        virtual ~HasSlot() { disconnect_all(); }

        size_t signals_count() const noexcept { return _signals.size(); }

        void disconnect(ISignal* obj) noexcept;

        template <typename Type, typename Member,
                  typename = std::enable_if_t<std::is_base_of<HasSlot, Type>::value>>
        void disconnect(ISignal* obj, Member Type::*member) noexcept;

        void disconnect_all() noexcept;

      private:
        template <typename F,
                  typename = std::enable_if_t<std::is_invocable_r_v<bool, F, const SignalConnect&>>>
        void _disconnect_if(F&& f) noexcept;

        void _add_signal(ISignal* sign, const _details::SlotInfo& info);
        void _remove_signal(ISignal* sign, size_t handle) noexcept;

      private:
        std::vector<SignalConnect> _signals;
    };

    template <typename F, typename>
    void HasSlot::_disconnect_if(F&& f) noexcept {
        for (const auto& sc : _signals) {
            if (f(sc)) {
                const auto& [sign, info] = sc;
                sign->_remove_slot_by_handle(info.handle);
            }
        }
        auto it = std::remove_if(_signals.begin(), _signals.end(), std::forward<F>(f));
        _signals.erase(it, _signals.end());
    }

    inline void HasSlot::_add_signal(ISignal* sign, const _details::SlotInfo& info) {
        _signals.push_back({sign, info});
    }

    inline void HasSlot::_remove_signal(ISignal* sign, size_t handle) noexcept {
        auto it = std::remove_if(_signals.begin(), _signals.end(), [=](const SignalConnect& sc) {
            const auto& [s, info] = sc;
            return s == sign && info.handle == handle;
        });
        _signals.erase(it, _signals.end());
    }

    inline void HasSlot::disconnect(ISignal* obj) noexcept {
        _disconnect_if([=](const SignalConnect& sc) {
            const auto& [sign, info] = sc;
            return sign == obj;
        });
    }

    template <typename Type, typename Member, typename>
    inline void HasSlot::disconnect(ISignal* obj, Member Type::*member) noexcept {
        _disconnect_if([=](const SignalConnect& sc) {
            const auto& [sign, info] = sc;
            return sign == obj && info.member_ptr == _details::to_member_type(member);
        });
    }

    inline void HasSlot::disconnect_all() noexcept {
        _disconnect_if([=](const SignalConnect& sc) { return true; });
    }

} // namespace RolUI
