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

    class IntrusivePrimeList {
      public:
        typedef IntrusiveListIterator iterator;
        typedef const IntrusiveListIterator const_iterator;

      public:
        IntrusivePrimeList() noexcept : _size(0), _first(nullptr), _last(nullptr) {}

        size_t size() const noexcept { return _size; }

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

        template <typename Type>
        IntrusiveView<iterator, Type, IntrusiveListNode>
        view(const IntrusiveListNode Type::*offset, bool reverse = false) noexcept {
            if (reverse)
                return IntrusiveView(rbegin(), rend(), offset);
            else
                return IntrusiveView(begin(), end(), offset);
        }
        template <typename Type>
        const IntrusiveView<iterator, Type, IntrusiveListNode>
        view(const IntrusiveListNode Type::*offset, bool reverse = false) const noexcept {
            if (reverse)
                return IntrusiveView(rbegin(), rend(), offset);
            else
                return IntrusiveView(begin(), end(), offset);
        }

      private:
        size_t _size;
        IntrusiveListNode* _first;
        IntrusiveListNode* _last;
    };

    template <typename T>
    class IntrusiveList {
      public:
        typedef IntrusiveTransformIterator<IntrusiveListIterator, T, IntrusiveListNode> iterator;
        typedef const IntrusiveTransformIterator<IntrusiveListIterator, T, IntrusiveListNode> const_iterator;

        typedef IntrusiveView<IntrusivePrimeList::iterator, T, IntrusiveListNode> View;

      public:
        IntrusiveList(IntrusiveListNode T::*member) noexcept : _member_offset(member) {}
        ~IntrusiveList() {}

        T* front() noexcept { return _contain_of(_list.front()); }
        T* back() noexcept { return _contain_of(_list.back()); }
        const T* front() const noexcept { return _contain_of(_list.front()); }
        const T* back() const noexcept { return _contain_of(_list.back()); }

        size_t size() const noexcept { return _list.size(); }

        IntrusiveListNode T::*member_offset() noexcept { return _member_offset; }
        const IntrusiveListNode T::*member_offset() const noexcept { return _member_offset; }

        IntrusivePrimeList& prime_list() noexcept { return _list; }
        const IntrusivePrimeList& prime_list() const noexcept { return _list; }

        void insert_prev(T* pos, T* node) noexcept {
            _list.insert_prev(&(pos->*_member_offset), &(node->*_member_offset));
        }
        void insert_next(T* pos, T* node) noexcept {
            _list.insert_next(&(pos->*_member_offset), &(node->*_member_offset));
        }

        void insert_front(T* node) noexcept {
            _list.insert_front(&(node->*_member_offset));
        }
        void insert_back(T* node) noexcept {
            _list.insert_back(&(node->*_member_offset));
        }

        void append(T* node) noexcept {
            _list.append(&(node->*_member_offset));
        }

        void remove(T* node) noexcept {
            _list.remove(&(node->*_member_offset));
        }

        iterator begin() noexcept { return iterator(_list.begin(), _member_offset); }
        iterator end() noexcept { return iterator(_list.end(), _member_offset); }
        const_iterator begin() const noexcept { return const_iterator(_list.begin(), _member_offset); }
        const_iterator end() const noexcept { return const_iterator(_list.end(), _member_offset); }

        const_iterator cbegin() const noexcept { return const_iterator(_list.begin(), _member_offset); }
        const_iterator cend() const noexcept { return const_iterator(_list.end(), _member_offset); }

        iterator rbegin() noexcept { return iterator(_list.rbegin(), _member_offset); }
        iterator rend() noexcept { return iterator(_list.rend(), _member_offset); }
        const_iterator rbegin() const noexcept { return const_iterator(_list.rbegin(), _member_offset); }
        const_iterator rend() const noexcept { return const_iterator(_list.rend(), _member_offset); }

        View view(bool reverse = false) noexcept {
            return _list.view(_member_offset, reverse);
        }
        const View view(bool reverse = false) const noexcept {
            return _list.view(_member_offset, reverse);
        }

      private:
        T* _contain_of(IntrusiveListNode* node) {
            return node ? contain_of(node, _member_offset) : nullptr;
        }

      private:
        IntrusiveListNode T::*_member_offset;
        IntrusivePrimeList _list;
    };
} // namespace RolUI
