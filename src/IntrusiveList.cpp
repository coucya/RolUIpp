

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

    void IntrusiveListNode::remove_self() noexcept {
        IntrusiveListNode* p = _prev;
        IntrusiveListNode* n = _next;

        if (p != nullptr)
            p->_next = n;
        if (n != nullptr)
            n->_prev = p;

        _prev = nullptr;
        _next = nullptr;
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

    void IntrusivePrimeList::insert_prev(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept {
        if (pos == nullptr) return;
        if (node == nullptr) return;

        pos->insert_prev(node);
        _size++;

        if (pos == _first) _first = node;
    }
    void IntrusivePrimeList::insert_next(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept {
        if (pos == nullptr) return;
        if (node == nullptr) return;

        pos->insert_next(node);
        _size++;

        if (pos == _last) _last = node;
    }

    void IntrusivePrimeList::insert_front(IntrusiveListNode* node) noexcept {
        if (node == nullptr) return;

        node->remove_self();

        _size++;

        if (_first == nullptr) {
            _first = node;
            _last = node;
            return;
        }

        _first->insert_prev(node);
        _first = node;
    }
    void IntrusivePrimeList::insert_back(IntrusiveListNode* node) noexcept {
        if (node == nullptr) return;

        node->remove_self();

        _size++;

        if (_last == nullptr) {
            _first = node;
            _last = node;
            return;
        }

        _last->insert_next(node);
        _last = node;
    }

    void IntrusivePrimeList::append(IntrusiveListNode* node) noexcept {
        insert_back(node);
    }

    void IntrusivePrimeList::remove(IntrusiveListNode* node) noexcept {
        if (node == _first)
            _first = node->next();
        if (node == _last)
            _last = node->prev();
        _size--;
        node->remove_self();
    }

    IntrusiveListIterator& IntrusiveListIterator::operator++() noexcept {
        if (_node != nullptr)
            _node = _is_reverse ? _node->prev() : _node->next();
        return *this;
    }
    IntrusiveListIterator& IntrusiveListIterator::operator--() noexcept {
        if (_node != nullptr)
            _node = _is_reverse ? _node->next() : _node->prev();
        return *this;
    }

    bool IntrusiveListIterator::operator==(const IntrusiveListIterator& other) const noexcept {
        return _node == other._node;
    }
    bool IntrusiveListIterator::operator!=(const IntrusiveListIterator& other) const noexcept {
        return _node != other._node;
    }
} // namespace RolUI
