#pragma once

#include <stdint.h>

namespace RolUI {

    class IntrusiveListNode {
      public:
        IntrusiveListNode() noexcept : _prev(nullptr), _next(nullptr) {}

        IntrusiveListNode* prev() noexcept { return _prev; }
        IntrusiveListNode* next() noexcept { return _next; }

        const IntrusiveListNode* prev() const noexcept { return _prev; }
        const IntrusiveListNode* next() const noexcept { return _next; }

        void insert_prev(IntrusiveListNode* node) noexcept {
            if (node == nullptr) return;

            node->remove_self();

            IntrusiveListNode* p = _prev;

            if (p != nullptr)
                p->_next = node;
            _prev = node;

            node->_prev = p;
            node->_next = this;
        }

        void insert_next(IntrusiveListNode* node) noexcept {
            if (node == nullptr) return;

            node->remove_self();

            IntrusiveListNode* n = _next;

            if (n != nullptr)
                n->_prev = node;
            _next = node;

            node->_prev = this;
            node->_next = n;
        }

        void remove_self() noexcept {
            IntrusiveListNode *p, *n;
            p = _prev;
            n = _next;

            if (p != nullptr)
                p->_next = n;
            if (n != nullptr)
                n->_prev = p;

            _prev = nullptr;
            _next = nullptr;
        }

        IntrusiveListNode* remove_prev() noexcept {
            IntrusiveListNode* p = _prev;
            if (p == nullptr) return nullptr;
            p->remove_self();
            return p;
        }
        IntrusiveListNode* remove_next() noexcept {
            IntrusiveListNode* n = _next;
            if (n == nullptr) return nullptr;
            n->remove_self();
            return n;
        }

      private:
        IntrusiveListNode* _prev;
        IntrusiveListNode* _next;
    };

    class IntrusiveList {
      public:
        IntrusiveList() noexcept : _first(nullptr), _last(nullptr) {}

        IntrusiveListNode* front() noexcept { return _first; }
        IntrusiveListNode* back() noexcept { return _last; }
        const IntrusiveListNode* front() const noexcept { return _first; }
        const IntrusiveListNode* back() const noexcept { return _last; }

        void insert_prev(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept {
            if (pos == nullptr) return;
            if (node == nullptr) return;

            pos->insert_prev(node);
            if (pos == _first) _first = node;
        }
        void insert_next(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept {
            if (pos == nullptr) return;
            if (node == nullptr) return;

            pos->insert_next(node);
            if (pos == _last) _last = node;
        }

        void insert_front(IntrusiveListNode* node) noexcept {
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
        void insert_back(IntrusiveListNode* node) noexcept {
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

        void append(IntrusiveListNode* node) noexcept {
            insert_back(node);
        }

        void remove(IntrusiveListNode* node) noexcept {
            if (node == _first)
                _first = node->next();
            if (node == _last)
                _last = node->prev();
            node->remove_self();
        }

      private:
        IntrusiveListNode* _first;
        IntrusiveListNode* _last;
    };

} // namespace RolUI
