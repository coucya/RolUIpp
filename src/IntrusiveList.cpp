

#include "RolUI/IntrusiveList.h"

namespace RolUI {
    void IntrusiveListNode::insert_prev(IntrusiveListNode* node) noexcept {
        if (node == nullptr) return;

        node->remove_self();

        IntrusiveListNode* p = _prev;

        if (p != nullptr)
            p->_next = node;
        _prev = node;

        node->_prev = p;
        node->_next = this;
    }
    void IntrusiveListNode::insert_next(IntrusiveListNode* node) noexcept {
        if (node == nullptr) return;

        node->remove_self();

        IntrusiveListNode* n = _next;

        if (n != nullptr)
            n->_prev = node;
        _next = node;

        node->_prev = this;
        node->_next = n;
    }

    IntrusiveListNode* IntrusiveListNode::remove_prev() noexcept {
        IntrusiveListNode* p = _prev;
        if (p == nullptr) return nullptr;
        p->remove_self();
        return p;
    }
    IntrusiveListNode* IntrusiveListNode::remove_next() noexcept {
        IntrusiveListNode* n = _next;
        if (n == nullptr) return nullptr;
        n->remove_self();
        return n;
    }

    void IntrusiveList::insert_prev(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept {
        if (pos == nullptr) return;
        if (node == nullptr) return;

        pos->insert_prev(node);
        if (pos == _first) _first = node;
    }
    void IntrusiveList::insert_next(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept {
        if (pos == nullptr) return;
        if (node == nullptr) return;

        pos->insert_next(node);
        if (pos == _last) _last = node;
    }

    void IntrusiveList::insert_front(IntrusiveListNode* node) noexcept {
        if (node == nullptr) return;

        node->remove_self();

        if (_first == nullptr) {
            _first = node;
            _last = node;
            return;
        }

        _first->insert_prev(node);
        _first = node;
    }
    void IntrusiveList::insert_back(IntrusiveListNode* node) noexcept {
        if (node == nullptr) return;

        node->remove_self();

        if (_last == nullptr) {
            _first = node;
            _last = node;
            return;
        }

        _last->insert_next(node);
        _last = node;
    }

    void IntrusiveList::append(IntrusiveListNode* node) noexcept {
        insert_back(node);
    }

    void IntrusiveList::remove(IntrusiveListNode* node) noexcept {
        if (node == _first)
            _first = node->next();
        if (node == _last)
            _last = node->prev();
        node->remove_self();
    }

    IntrusiveListIterator& IntrusiveListIterator::operator++() noexcept {
        if (_node != nullptr)
            if (_is_reverse)
                _node = _node->prev();
            else
                _node = _node->next();
        return *this;
    }
    IntrusiveListIterator& IntrusiveListIterator::operator--() noexcept {
        if (_node != nullptr)
            if (_is_reverse)
                _node = _node->next();
            else
                _node = _node->prev();
        return *this;
    }

    bool IntrusiveListIterator::operator==(const IntrusiveListIterator& other) const noexcept {
        return _node == other._node;
    }
    bool IntrusiveListIterator::operator!=(const IntrusiveListIterator& other) const noexcept {
        return _node != other._node;
    }
} // namespace RolUI
