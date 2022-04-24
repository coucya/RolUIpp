#pragma once

#include <type_traits>
#include <utility>

namespace RolUI {

    template <typename RT, typename FT, typename... Args>
    using enable_if_invocable_t = typename std::enable_if<std::is_invocable_r<RT, FT, Args...>::value>::type;

    namespace _details {

        class SlotBase;
        class HasSlotBase;
        class SignalBase;

        void connect(SignalBase* signal, HasSlotBase* hasslot, SlotBase* slot, unsigned handle) noexcept;
        void disconnect(SlotBase* slot) noexcept;

        template <typename SlotType, typename... Args>
        unsigned connect(SignalBase* signal, HasSlotBase* hasslot, Args&&... args) noexcept;

        class HasSlotBase {
            friend void connect(SignalBase* signal, HasSlotBase* hasslot, SlotBase* slot, unsigned handle) noexcept;
            friend void disconnect(SlotBase* slot) noexcept;

          public:
            HasSlotBase() noexcept {}
            ~HasSlotBase() {}

          protected:
            unsigned signal_count() const noexcept { return _slot_count; }

            template <typename F, typename = enable_if_invocable_t<void, F, SlotBase*>>
            void for_each(F&& f) const noexcept;

          private:
            void _insert_back(_details::SlotBase* slot) noexcept;
            void _remove(_details::SlotBase* slot) noexcept;

            unsigned _slot_count = 0;
            _details::SlotBase* _slot_first = nullptr;
            _details::SlotBase* _slot_last = nullptr;
        };

        class SignalBase {
            friend void connect(SignalBase* signal, HasSlotBase* hasslot, SlotBase* slot, unsigned handle) noexcept;
            friend void disconnect(SlotBase* slot) noexcept;
            template <typename SlotType, typename... Args>
            friend unsigned connect(SignalBase*, HasSlotBase*, Args&&...) noexcept;

          public:
            SignalBase() noexcept {}
            ~SignalBase() {}

          protected:
            unsigned slot_count() const noexcept { return _slot_count; }
            unsigned alloc_handle() noexcept { return ++_handle; }

            template <typename F, typename = enable_if_invocable_t<void, F, SlotBase*>>
            void for_each(F&& f) const noexcept;

          private:
            void
            _insert_back(_details::SlotBase* slot) noexcept;
            void _remove(_details::SlotBase* slot) noexcept;

            unsigned _handle = 0;
            unsigned _slot_count = 0;
            _details::SlotBase* _signal_first = nullptr;
            _details::SlotBase* _signal_last = nullptr;
        };

        class SlotBase {
            friend void connect(SignalBase* signal, HasSlotBase* hasslot, SlotBase* slot, unsigned handle) noexcept;
            friend void disconnect(SlotBase* slot) noexcept;

            friend class SignalBase;
            friend class HasSlotBase;

          public:
            virtual ~SlotBase() {}

            unsigned handle() const noexcept { return _handle; }
            SignalBase* signal() const noexcept { return _signal; }
            HasSlotBase* HasSlot() const noexcept { return _hasslot; }

          private:
            unsigned _handle = 0;

            SignalBase* _signal = nullptr;
            HasSlotBase* _hasslot = nullptr;

            SlotBase* _slot_prev = nullptr;
            SlotBase* _slot_next = nullptr;

            SlotBase* _signal_prev = nullptr;
            SlotBase* _signal_next = nullptr;
        };

        template <typename... Args>
        class ISlot : public SlotBase {
          public:
            ~ISlot() override {}
            virtual void apply(Args... args) const = 0;
        };

        template <typename F, typename... Args>
        class Slot : public ISlot<Args...> {
          public:
            Slot() noexcept {}
            explicit Slot(const F& callable) noexcept : _callable(callable) {}
            explicit Slot(F&& callable) noexcept : _callable(std::move(callable)) {}

            ~Slot() override {}

            void apply(Args... args) const override { _callable(args...); }

          private:
            F _callable;
        };

        template <typename F, typename>
        void HasSlotBase::for_each(F&& f) const noexcept {
            _details::SlotBase* it = _slot_first;
            for (; it; it = it->_slot_next)
                f(it);
        }

        template <typename F, typename>
        void SignalBase::for_each(F&& f) const noexcept {
            _details::SlotBase* it = _signal_first;
            for (; it; it = it->_signal_next)
                f(it);
        }

        inline void HasSlotBase::_insert_back(SlotBase* slot) noexcept {
            slot->_slot_prev = slot->_slot_next = nullptr;

            if (_slot_first == nullptr || _slot_last == nullptr) {
                _slot_first = _slot_last = slot;
            } else {
                _slot_last->_slot_next = slot;
                slot->_slot_prev = _slot_last;
                _slot_last = slot;
            }
            slot->_hasslot = this;

            _slot_count++;
        }

        inline void HasSlotBase::_remove(SlotBase* slot) noexcept {
            if (slot->_hasslot != this) return;

            if (_slot_first == nullptr || _slot_last == nullptr) {
                _slot_first = _slot_last = nullptr;
            } else {
                SlotBase* p = slot->_slot_prev;
                SlotBase* n = slot->_slot_next;

                if (_slot_first == slot)
                    _slot_first = n;
                if (_slot_last == slot)
                    _slot_last = p;

                if (p) p->_slot_next = n;
                if (n) n->_slot_prev = p;

                slot->_slot_prev = slot->_slot_next = nullptr;
                _slot_count--;
            }
            slot->_hasslot = nullptr;
        }

        inline void SignalBase::_insert_back(SlotBase* slot) noexcept {
            slot->_signal_prev = slot->_signal_next = nullptr;

            if (_signal_first == nullptr || _signal_last == nullptr) {
                _signal_first = _signal_last = slot;
            } else {
                _signal_last->_signal_next = slot;
                slot->_signal_prev = _signal_last;
                _signal_last = slot;
            }
            slot->_signal = this;

            _slot_count++;
        }

        inline void SignalBase::_remove(SlotBase* slot) noexcept {
            if (!slot || slot->_signal != this) return;

            if (_signal_first == nullptr || _signal_last == nullptr) {
                _signal_first = _signal_last = nullptr;
            } else {
                SlotBase* p = slot->_signal_prev;
                SlotBase* n = slot->_signal_next;

                if (_signal_first == slot)
                    _signal_first = n;
                if (_signal_last == slot)
                    _signal_last = p;

                if (p) p->_signal_next = n;
                if (n) n->_signal_prev = p;

                slot->_signal_prev = slot->_signal_next = nullptr;

                _slot_count--;
            }
            slot->_signal = nullptr;
        }

        inline void connect(SignalBase* signal, HasSlotBase* hasslot, SlotBase* slot, unsigned handle) noexcept {
            slot->_handle = handle;
            slot->_signal = nullptr;
            slot->_hasslot = nullptr;
            if (signal) signal->_insert_back(slot);
            if (hasslot) hasslot->_insert_back(slot);
        }
        inline void disconnect(SlotBase* slot) noexcept {
            if (!slot) return;
            if (slot->_signal) slot->_signal->_remove(slot);
            if (slot->_hasslot) slot->_hasslot->_remove(slot);
            delete slot;
        }
        template <typename SlotType, typename... Args>
        unsigned connect(SignalBase* signal, HasSlotBase* hasslot, Args&&... args) noexcept {
            if (!signal) return 0;
            unsigned handle = signal->alloc_handle();
            SlotBase* slot = new SlotType{std::forward<Args>(args)...};
            _details::connect(signal, hasslot, slot, handle);
            return handle;
        }

    } // namespace _details

    class HasSlot : public _details::HasSlotBase {
      public:
        HasSlot() noexcept {}
        virtual ~HasSlot() { disconnect_all(); }

        HasSlot(HasSlot&&) = delete;
        HasSlot(const HasSlot&) = delete;
        HasSlot& operator=(HasSlot&&) = delete;
        HasSlot& operator=(const HasSlot&) = delete;

        unsigned signals_count() const noexcept { return HasSlotBase::signal_count(); }

        void disconnect(_details::SignalBase* signal, unsigned handle) noexcept {
            _details::SlotBase* slot = nullptr;
            for_each([&](_details::SlotBase* s) {
                if (s->handle() == handle && s->signal() == signal)
                    slot = s;
            });
            _details::disconnect(slot);
        }
        void disconnect_all() noexcept {
            _details::SlotBase* last = nullptr;
            for_each([&](_details::SlotBase* s) {
                if (last) _details::disconnect(last);
                last = s;
            });
            if (last) _details::disconnect(last);
        }
    };

    template <typename... Args>
    class Signal : public _details::SignalBase, public HasSlot {
      public:
        typedef void (*CallbackFunc)(Args...);

      public:
        Signal() noexcept {}
        ~Signal() { Signal::disconnect_all(); }

        unsigned connect(CallbackFunc fp) noexcept;

        template <typename Type, typename Member,
                  typename = enable_if_invocable_t<void, Member, Type*, Args...>>
        unsigned connect(Type* obj, Member member) noexcept;

        template <typename Type,
                  typename = typename std::enable_if<std::is_copy_constructible<Type>::value>::type,
                  typename = enable_if_invocable_t<void, Type, Args...>>
        unsigned connect(const Type& callable) noexcept;

        template <typename Type,
                  typename = typename std::enable_if<std::is_move_constructible<Type>::value>::type,
                  typename = enable_if_invocable_t<void, Type, Args...>>
        unsigned connect(Type&& callable) noexcept;

        template <typename Type,
                  typename = enable_if_invocable_t<void, Type, Args...>>
        unsigned connect(Type* callable) noexcept;

        unsigned slot_count() const noexcept { return SignalBase::slot_count(); }

        void disconnect(unsigned handle) noexcept;
        void disconnect_all() noexcept;

        void emit(Args... args) const noexcept;
    };

    template <typename... Args>
    unsigned Signal<Args...>::connect(typename Signal<Args...>::CallbackFunc fp) noexcept {
        if (!fp) return 0;
        return _details::connect<_details::Slot<CallbackFunc, Args...>>(this, nullptr, fp);
    }

    template <typename... Args>
    template <typename Type, typename Member, typename>
    unsigned Signal<Args...>::connect(Type* obj, Member member) noexcept {
        if (!obj) return 0;
        auto l = [=](Args... args) { (obj->*member)(args...); };
        if (std::is_base_of<_details::HasSlotBase, Type>::value)
            return _details::connect<_details::Slot<decltype(l), Args...>>(this, static_cast<_details::HasSlotBase*>(obj), std::move(l));
        else
            return _details::connect<_details::Slot<decltype(l), Args...>>(this, nullptr, std::move(l));
    }

    template <typename... Args>
    template <typename Type, typename, typename>
    unsigned Signal<Args...>::connect(const Type& callable) noexcept {
        return _details::connect<_details::Slot<Type, Args...>>(this, nullptr, callable);
    }

    template <typename... Args>
    template <typename Type, typename, typename>
    unsigned Signal<Args...>::connect(Type&& callable) noexcept {
        return _details::connect<_details::Slot<Type, Args...>>(this, nullptr, std::move(callable));
    }

    template <typename... Args>
    template <typename Type, typename>
    unsigned Signal<Args...>::connect(Type* callable) noexcept {
        if (!callable) return 0;
        auto l = [=](Args... args) { (*callable)(args...); };
        if (std::is_base_of<_details::HasSlotBase, Type>::value)
            return _details::connect<_details::Slot<decltype(l), Args...>>(this, static_cast<_details::HasSlotBase*>(callable), std::move(l));
        else
            return _details::connect<_details::Slot<decltype(l), Args...>>(this, nullptr, std::move(l));
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(unsigned handle) noexcept {
        _details::SlotBase* slot = nullptr;
        SignalBase::for_each([&](_details::SlotBase* s) {
            if (s->handle() == handle) slot = s;
        });
        if (slot) _details::disconnect(slot);
    }
    template <typename... Args>
    void Signal<Args...>::disconnect_all() noexcept {
        _details::SlotBase* last = nullptr;
        SignalBase::for_each([&](_details::SlotBase* s) {
            if (last) _details::disconnect(last);
            last = s;
        });
        if (last) _details::disconnect(last);
    }

    template <typename... Args>
    void Signal<Args...>::emit(Args... args) const noexcept {
        SignalBase::for_each([&](_details::SlotBase* s) {
            _details::ISlot<Args...>* is = static_cast<_details::ISlot<Args...>*>(s);
            is->apply(args...);
        });
    }

} // namespace RolUI
