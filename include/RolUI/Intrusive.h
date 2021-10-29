#pragma once

#include <stdint.h>
#include <type_traits>

namespace RolUI {

    template <typename Type, typename Member>
    Type* contain_of(const Member* member, const Member Type::*offset) {
        return (Type*)((uint8_t*)member - (size_t) & (((Type*)0)->*offset));
    }

    template <typename Iter, typename Type, typename Member>
    class IntrusiveTransformIterator {
        static_assert(std::is_same<typename Iter::value_type, Member>::value);

      public:
        typedef typename Iter::iterator_category iterator_category;
        typedef Type value_type;
        typedef Type* pointer;
        typedef Type& reference;
        typedef typename Iter::difference_type difference_type;

        IntrusiveTransformIterator(Iter iter, const Member Type::*offset) noexcept
            : _iter(iter), _offset(offset) {}

        IntrusiveTransformIterator(const IntrusiveTransformIterator&) = default;
        IntrusiveTransformIterator(IntrusiveTransformIterator&&) = default;

        ~IntrusiveTransformIterator() noexcept {}

        IntrusiveTransformIterator& operator=(const IntrusiveTransformIterator&) = default;
        IntrusiveTransformIterator& operator=(IntrusiveTransformIterator&&) = default;

        IntrusiveTransformIterator& operator++() noexcept {
            ++_iter;
            return *this;
        }
        IntrusiveTransformIterator& operator--() noexcept {
            --_iter;
            return *this;
        }

        bool operator==(const IntrusiveTransformIterator& other) const noexcept {
            return _iter == other._iter;
        }
        bool operator!=(const IntrusiveTransformIterator& other) const noexcept {
            return _iter != other._iter;
        }

        reference operator*() noexcept { return *contain_of(&*_iter, _offset); }
        // const reference operator*() const noexcept { return *contain_of(&*_iter, _offset); }

        pointer operator->() noexcept { return contain_of(&*_iter, _offset); }
        const pointer operator->() const noexcept { return contain_of(&*_iter, _offset); }

      private:
        Iter _iter;
        const Member Type::*_offset;
    };

    template <typename Iter, typename Type, typename Member>
    class IntrusiveView {
      public:
        typedef IntrusiveTransformIterator<Iter, Type, Member> iterator;
        typedef const IntrusiveTransformIterator<Iter, Type, Member> const_iterator;

        IntrusiveView(const iterator& b, const iterator& e) noexcept
            : _begin(b), _end(e) {}
        IntrusiveView(const Iter& b, const Iter& e, const Member Type::*offset) noexcept
            : _begin(b, offset), _end(e, offset) {}

        iterator begin() noexcept { return _begin; }
        iterator end() noexcept { return _end; }

        const_iterator begin() const noexcept { return _begin; }
        const_iterator end() const noexcept { return _end; }

        const_iterator cbegin() const noexcept { return _begin; }
        const_iterator cend() const noexcept { return _end; }

      private:
        iterator _begin, _end;
    };

} // namespace RolUI
