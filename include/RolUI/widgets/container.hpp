#pragma once

#include "../Color.hpp"
#include "../Rect.hpp"
#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        enum class SizeUnitType {
            pixel,
            percentage
        };
        class SizeUnit {
          public:
            SizeUnit() noexcept : _tp(SizeUnitType::percentage), _per(1.0f) {}

            bool operator==(const SizeUnit& su) const noexcept {
                return _tp == su._tp && _px == su._px;
            }
            bool operator!=(const SizeUnit& su) const noexcept {
                return _tp != su._tp || _px != su._px;
            }

            SizeUnitType type() const noexcept { return _tp; }
            int pixel() const noexcept { return _px; }
            float percentage() const noexcept { return _per; }

          public:
            static SizeUnit pixel(int p) noexcept {
                SizeUnit su;
                su._px = p, su._tp = SizeUnitType::pixel;
                return su;
            }
            static SizeUnit percentage(float p) noexcept {
                SizeUnit su;
                su._per = p, su._tp = SizeUnitType::percentage;
                return su;
            }

          private:
            SizeUnitType _tp;
            union {
                float _per;
                int _px;
            };
        };

        class Box : public SingleChildWidget {
          public:
            Property<unsigned> round{this, 0};
            Property<unsigned> border_width{this, 0};
            Property<Color> border_color{this, {0, 0, 0, 255}};
            Property<Color> background_color{this, {255, 255, 255, 255}};

          public:
            Box(unsigned round = 0) noexcept;
            ~Box() override;

          protected:
            virtual void draw(IPainter* painter) noexcept override;
        };

        class SizedBox : public SingleChildWidget {
          public:
            Property<SizeUnit> width{this, SizeUnit::percentage(1.0f)};
            Property<SizeUnit> height{this, SizeUnit::percentage(1.0f)};

          public:
            SizedBox() noexcept;
            SizedBox(SizeUnit w, SizeUnit h) noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };

        class Align : public SingleChildWidget {
          public:
            Property<float> align_x{this, 0.5f};
            Property<float> align_y{this, 0.5f};

          public:
            Align() noexcept;
            Align(float x, float y) noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };

        class Margin : public SingleChildWidget {
          public:
            Property<unsigned> top{this, 0};
            Property<unsigned> bottom{this, 0};
            Property<unsigned> left{this, 0};
            Property<unsigned> right{this, 0};

          public:
            Margin() noexcept;
            Margin(unsigned margin) noexcept;
            Margin(unsigned x, unsigned y) noexcept;
            Margin(unsigned top, unsigned bottom, unsigned left, unsigned right) noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI