#pragma once

#include <iterator>
#include <stdint.h>

#include "Intrusive.h"

namespace RolUI {

    class IntrusiveListNode {
      public:
        IntrusiveListNode() noexcept : _prev(nullptr), _next(nullptr) {}

        IntrusiveListNode* prev() noexcept { return _prev; }
        IntrusiveListNode* next() noexcept { return _next; }

        const IntrusiveListNode* prev() const noexcept { return _prev; }
        const IntrusiveListNode* next() const noexcept { return _next; }

        void insert_prev(IntrusiveListNode* node) noexcept;
        void insert_next(IntrusiveListNode* node) noexcept;

        void remove_self() noexcept;

        IntrusiveListNode* remove_prev() noexcept;
        IntrusiveListNode* remove_next() noexcept;

      private:
        IntrusiveListNode* _prev;
        IntrusiveListNode* _next;
    };

    class IntrusiveListIterator {
      public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef IntrusiveListNode value_type;
        typedef IntrusiveListNode* pointer;
        typedef IntrusiveListNode& reference;
        typedef void difference_type;

        IntrusiveListIterator() noexcept : _node(nullptr), _is_reverse(false) {}
        IntrusiveListIterator(IntrusiveListNode* node, bool reverse = false) noexcept
            : _node(node), _is_reverse(reverse) {}

        IntrusiveListIterator(const IntrusiveListIterator&) = default;
        IntrusiveListIterator(IntrusiveListIterator&&) = default;

        ~IntrusiveListIterator() noexcept {}

        IntrusiveListIterator& operator=(const IntrusiveListIterator&) = default;
        IntrusiveListIterator& operator=(IntrusiveListIterator&&) = default;

        IntrusiveListIterator& operator++() noexcept;
        IntrusiveListIterator& operator--() noexcept;

        bool operator==(const IntrusiveListIterator& other) const noexcept;
        bool operator!=(const IntrusiveListIterator& other) const noexcept;

        reference operator*() noexcept { return *_node; }
        const reference operator*() const noexcept { return *_node; }

        pointer operator->() noexcept { return _node; }
        const pointer operator->() const noexcept { return _node; }

      private:
        bool _is_reverse;
        IntrusiveListNode* _node;
    };

    class IntrusiveList {
      public:
        typedef IntrusiveListIterator iterator;
        typedef const IntrusiveListIterator const_iterator;

        IntrusiveList() noexcept : _first(nullptr), _last(nullptr) {}

        IntrusiveListNode* front() noexcept { return _first; }
        IntrusiveListNode* back() noexcept { return _last; }
        const IntrusiveListNode* front() const noexcept { return _first; }
        const IntrusiveListNode* back() const noexcept { return _last; }

        void insert_prev(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept;
        void insert_next(IntrusiveListNode* pos, IntrusiveListNode* node) noexcept;

        void insert_front(IntrusiveListNode* node) noexcept;
        void insert_back(IntrusiveListNode* node) noexcept;

        void append(IntrusiveListNode* node) noexcept;

        void remove(IntrusiveListNode* node) noexcept;

        iterator begin() noexcept { return {_first}; }
        iterator end() noexcept { return {nullptr}; }
        const_iterator begin() const noexcept { return {_first}; }
        const_iterator end() const noexcept { return {nullptr}; }

        const_iterator cbegin() const noexcept { return {_first}; }
        const_iterator cend() const noexcept { return {nullptr}; }

        iterator rbegin() noexcept { return {_last, true}; }
        iterator rend() noexcept { return {nullptr, true}; }
        const_iterator rbegin() const noexcept { return {_last, true}; }
        const_iterator rend() const noexcept { return {nullptr, true}; }

        template <typename Type, typename Member>
        IntrusiveView<iterator, Type, Member> view(const Member Type::*offset) noexcept {
            return IntrusiveView(begin(), end(), offset);
        }

        template <typename Type, typename Member>
        const IntrusiveView<iterator, Type, Member> view(const Member Type::*offset) const noexcept {
            return IntrusiveView(begin(), end(), offset);
        }

      private:
        IntrusiveListNode* _first;
        IntrusiveListNode* _last;
    };

} // namespace RolUI
