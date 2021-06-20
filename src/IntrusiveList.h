#pragma once

#include <iterator>
#include <stdint.h>

namespace RolUI {

    template <typename Type, typename Member>
    Type* contain_of(const Member* member, const Member Type::*offset) {
        void* obj_ptr = (void*)member - (uint32_t)offset;
        return (Type*)obj_ptr;
    }

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

    class IntrusivelistIterator {
      public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef typename IntrusiveListNode value_type;
        typedef typename IntrusiveListNode* pointer;
        typedef typename IntrusiveListNode& reference;
        typedef void difference_type;

        IntrusivelistIterator() noexcept : _node(nullptr), _is_reverse(false) {}
        IntrusivelistIterator(IntrusiveListNode* node, bool reverse = false) noexcept
            : _node(node), _is_reverse(reverse) {}

        IntrusivelistIterator(const IntrusivelistIterator&) = default;
        IntrusivelistIterator(IntrusivelistIterator&&) = default;

        ~IntrusivelistIterator() noexcept {}

        IntrusivelistIterator& operator=(const IntrusivelistIterator&) = default;
        IntrusivelistIterator& operator=(IntrusivelistIterator&&) = default;

        IntrusivelistIterator& operator++() noexcept;
        IntrusivelistIterator& operator--() noexcept;

        bool operator==(const IntrusivelistIterator& other) const noexcept;
        bool operator!=(const IntrusivelistIterator& other) const noexcept;

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
        typedef IntrusivelistIterator iterator;
        typedef const IntrusivelistIterator const_iterator;

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

      private:
        IntrusiveListNode* _first;
        IntrusiveListNode* _last;
    };

} // namespace RolUI
