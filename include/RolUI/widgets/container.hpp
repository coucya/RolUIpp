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
            SizeUnit(int px) noexcept : _tp(SizeUnitType::pixel), _px(px) {}
            SizeUnit(unsigned px) noexcept : _tp(SizeUnitType::pixel), _px(px) {}
            SizeUnit(float per) noexcept : _tp(SizeUnitType::percentage), _per(per) {}
            SizeUnit(double per) noexcept : _tp(SizeUnitType::percentage), _per(per) {}

            bool operator==(const SizeUnit& su) const noexcept {
                return _tp == su._tp && _px == su._px;
            }
            bool operator!=(const SizeUnit& su) const noexcept {
                return _tp != su._tp || _px != su._px;
            }

            SizeUnitType type() const noexcept { return _tp; }
            int pixel() const noexcept { return _px; }
            float percentage() const noexcept { return _per; }

          private:
            SizeUnitType _tp;
            union {
                float _per;
                int _px;
            };
        };

        class BoxWidget : public SingleChildWidget {
          public:
            Property<BoxWidget, unsigned> round{this};
            Property<BoxWidget, unsigned> border_width{this, 0};
            Property<BoxWidget, Color> border_color{this, {0, 0, 0, 255}};
            Property<BoxWidget, Color> background_color{this, {255, 255, 255, 255}};

          public:
            BoxWidget() noexcept;
            ~BoxWidget() override;

          protected:
            void draw(IPainter* painter) noexcept override;
        };

        class SizedWidget : public SingleChildWidget {
          public:
            Property<SizedWidget, SizeUnit> width{this, SizeUnit(1.0f)};
            Property<SizedWidget, SizeUnit> height{this, SizeUnit(1.0f)};

          public:
            SizedWidget() noexcept;
            SizedWidget(SizeUnit w, SizeUnit h) noexcept;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class AlignWidget : public SingleChildWidget {
          public:
            Property<AlignWidget, float> align_x{this, 0.0f};
            Property<AlignWidget, float> align_y{this, 0.0f};

          public:
            AlignWidget() noexcept;
            AlignWidget(float x, float y) noexcept;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class MarginWidget : public SingleChildWidget {
          public:
            Property<MarginWidget, unsigned> top{this, 0};
            Property<MarginWidget, unsigned> bottom{this, 0};
            Property<MarginWidget, unsigned> left{this, 0};
            Property<MarginWidget, unsigned> right{this, 0};

          public:
            MarginWidget() noexcept;
            MarginWidget(unsigned margin) noexcept;
            MarginWidget(unsigned x, unsigned y) noexcept;
            MarginWidget(unsigned top, unsigned bottom, unsigned left, unsigned right) noexcept;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI
