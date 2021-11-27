
#include <cstddef>
#include <unordered_map>

#include "RolUI/Application.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Image.hpp"

namespace RolUI {

    static std::unordered_map<size_t, size_t> image_rc;

    static size_t image_new_rgba_mem(const uint8_t* data, size_t w, size_t h) {
        size_t handle = Application::window()->painter()->create_image_with_rgba(data, w, h);
        image_rc[handle] = 1;
        return handle;
    }
    static void image_inc(size_t handle) {
        auto it = image_rc.find(handle);
        if (it == image_rc.end()) return;
        it->second++;
    }
    static void image_dec(size_t handle) {
        auto it = image_rc.find(handle);
        if (it == image_rc.end()) return;
        it->second--;

        if (it->second == 0) {
            Application::window()->painter()->delete_image(it->first);
            image_rc.erase(it);
        }
    }

    Image Image::create_rgba_mem(const uint8_t* data, size_t w, size_t h) {
        size_t handle = image_new_rgba_mem(data, w, h);
        return Image(handle);
    }

    Image::Image() noexcept : _has_image(false), _handle(0) {}

    Image::Image(size_t handle) noexcept : _has_image(true), _handle(handle) {}

    Image::Image(const Image& other) noexcept
        : _has_image(other._has_image), _handle(other._handle) {
        if (_has_image) image_inc(_handle);
    }
    Image::Image(Image&& other) noexcept
        : _has_image(other._has_image), _handle(other._handle) {
        if (_has_image) image_inc(_handle);
    }

    Image& Image::operator=(const Image& other) noexcept {
        if (_has_image) image_dec(_handle);
        _has_image = other._has_image;
        _handle = other._handle;
        return *this;
    }
    Image& Image::operator=(Image&& other) noexcept {
        if (_has_image) image_dec(_handle);
        _has_image = other._has_image;
        _handle = other._handle;
        return *this;
    }

    size_t Image::handle() const noexcept { return _handle; }

} // namespace RolUI
