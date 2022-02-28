#pragma once

#include <cstddef>
#include <cstdint>

namespace RolUI {

    class Image {
      public:
        static Image create_rgba_mem(const uint8_t* data, size_t w, size_t h);

      public:
        Image() noexcept;
        Image(const Image& other) noexcept;
        Image(Image&& other) noexcept;

        ~Image() noexcept;

        Image& operator=(const Image& other) noexcept;
        Image& operator=(Image&& other) noexcept;

        size_t handle() const noexcept;

      private:
        Image(size_t handle) noexcept;

      private:
        bool _has_image = false;
        size_t _handle = 0;
    };

} // namespace RolUI
