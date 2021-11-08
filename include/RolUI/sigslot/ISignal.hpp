#pragma once

#include <cstddef>

namespace RolUI {

    class HasSlot;

    namespace _details {

        typedef void (HasSlot::*MemberType)();

        template <typename Type, typename Member>
        MemberType to_member_type(Member Type::*member) noexcept {
            return reinterpret_cast<MemberType>(member);
        };

        struct SlotInfo {
            void* obj_ptr;
            MemberType member_ptr;
            HasSlot* slot_ptr;
            size_t handle;
        };

        inline SlotInfo make_slot_info(
            size_t handle,
            void* obj_ptr = nullptr,
            MemberType member_ptr = nullptr,
            HasSlot* solt_ptr = nullptr) {
            return {obj_ptr, member_ptr, solt_ptr, handle};
        }

    } // namespace _details

    class ISignal {
        friend class HasSlot;

      public:
        virtual ~ISignal() {}

      protected:
        size_t _alloc_handle() noexcept { return ++_handle; }

        virtual void _remove_slot_by_handle(size_t handle) noexcept = 0;

      private:
        size_t _handle = 0;
    };

} // namespace RolUI
