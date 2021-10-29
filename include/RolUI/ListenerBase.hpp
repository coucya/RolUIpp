#pragma once

#include <stdint.h>

#include "IEvent.h"
#include "IEventListener.h"
#include "IntrusiveList.h"

namespace RolUI {

    class ListenerBase : public IEventListener {
      public:
        typedef bool (*CallbackFun)(IEvent* e);

      public:
        static constexpr uint32_t max_linstener_number = 64;

      public:
        ListenerBase() noexcept;
        ~ListenerBase();

        void add_listener(EventType et, CallbackFun cb);
        void remove_listener(EventType et, CallbackFun cb);

        virtual bool on_event(IEvent* e) override;

      private:
        struct _ListenerNode {
            IntrusiveListNode brother;
            EventType event_type;
            CallbackFun callback;

            _ListenerNode() noexcept : event_type(), callback(nullptr) {}
            _ListenerNode(EventType et, CallbackFun cb) noexcept
                : event_type(et), callback(cb) {}
            void clear() { *this = {}; }
        };

        IntrusiveList<_ListenerNode> _free_list;
        IntrusiveList<_ListenerNode> _in_use_list;
        _ListenerNode _linsteners[max_linstener_number];
    };

} // namespace RolUI
