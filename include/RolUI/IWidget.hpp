#pragma once

#include <limits>

#include "./Size.hpp"
#include "./Point.hpp"

namespace RolUI {

    class IPainter;
    // class Style;

    class Constraint {
      public:
        static Constraint zero() noexcept { return Constraint{}; }
        static Constraint zero_to(Size max) noexcept { return Constraint({0, 0}, max); }
        static Constraint zero_to(int w, int h) noexcept { return Constraint(0, 0, w, h); }
        static Constraint limit(Size s) noexcept { return Constraint(s, s); }
        static Constraint limit(int w, int h) noexcept { return Constraint(w, h, w, h); }
        static Constraint limit(Size min, Size max) noexcept { return Constraint{min, max}; }
        static Constraint limit(int min_w, int min_h, int max_w, int max_h) noexcept {
            return Constraint(min_w, min_h, max_w, max_h);
        }
        static Constraint unlimit() noexcept {
            int imax = std::numeric_limits<int>::max();
            return Constraint(0, 0, imax, imax);
        }

      public:
        Constraint() noexcept : _min(0, 0), _max(0, 0) {}
        Constraint(Size min, Size max) noexcept : _min(min), _max(max) {}
        Constraint(int min_w, int min_h, int max_w, int max_h) noexcept
            : _min(min_w, min_h), _max(max_w, max_h) {}

        Size min() const noexcept { return _min; }
        Size max() const noexcept { return _max; }

        int max_width() const noexcept { return _max.width; }
        int max_height() const noexcept { return _max.height; }
        int min_width() const noexcept { return _min.width; }
        int min_height() const noexcept { return _min.height; }

      private:
        Size _min;
        Size _max;
    };

    class IWidget {
      public:
        virtual ~IWidget() {}

      protected:
        virtual void on_draw(IPainter* painter) {}

        virtual Size perlayout(Constraint constraint) = 0;
        virtual bool hit_test(Point pos) = 0;
    };

} // namespace RolUI
