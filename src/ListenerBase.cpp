
#include "RolUI/ListenerBase.h"

namespace RolUI {

    ListenerBase::ListenerBase() noexcept
        : _in_use_list(&_ListenerNode::brother), _free_list(&_ListenerNode::brother) {
        for (int i = 0; i < ListenerBase::max_linstener_number; i++) {
            _ListenerNode* node = &_linsteners[i];
            _free_list.insert_back(node);
        }
    }
    ListenerBase::~ListenerBase() {}

    void ListenerBase::add_listener(EventType et, CallbackFun cb) {
        if (cb == nullptr) return;

        _ListenerNode* node = _free_list.front();
        if (node == nullptr) return;

        node->event_type = et;
        node->callback = cb;

        _free_list.remove(node);
        _in_use_list.append(node);
    }
    void ListenerBase::remove_listener(EventType et, CallbackFun cb) {
        if (cb == nullptr) return;

        for (auto& node : _in_use_list) {
            if (node.event_type == et && node.callback == cb) {
                node.clear();
                _in_use_list.remove(&node);
                _free_list.append(&node);
            }
        }
        return;
    }

    bool ListenerBase::on_event(IEvent* e) {
        bool res = false;

        for (auto& node : _in_use_list) {
            if (node.event_type == e->event_type())
                res = (node.callback ? node.callback(e) : false) || res;
        }
        return res;
    }
} // namespace RolUI
