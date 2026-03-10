#define USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

/*
Пример использования библиотеки. Он будет компилироваться и работать, когда вы реализуете
все классы библиотеки.
*/

namespace shapes {

class Triangle : public Drawable {
public:
    Triangle(const Point p1, const Point p2, const Point p3)
        : p1_(p1), p2_(p2), p3_(p3) {}

    void Draw(ObjectContainer& container) const override {
        container.Add(Polyline()
            .AddPoint(p1_)
            .AddPoint(p2_)
            .AddPoint(p3_)
            .AddPoint(p1_)
        );
    }

private:
    Point p1_;
    Point p2_;
    Point p3_;
};


Polyline CreateStar(const Point center, const double outer_rad, const double inner_rad, const int num_rays) {
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
    return polyline;
}

class Star : public Drawable {
public:
    Star(const Point center, const double outer_rad, const double inner_rad, const int num_rays)
        : center_(center), outer_rad_(outer_rad), inner_rad_(inner_rad), num_rays_(num_rays) {}

    void Draw(ObjectContainer& container) const override {
        container.Add(CreateStar(center_, outer_rad_, inner_rad_, num_rays_)
            .SetFillColor("red")
            .SetStrokeColor("black"));
    }

private:
    Point center_;
    double outer_rad_;
    double inner_rad_;
    int num_rays_;
};

class Snowman : public Drawable {
public:
    Snowman(const Point center, const int head_r) : center_(center), head_r_(head_r) {}

    void Draw(ObjectContainer& container) const override {
        container.Add(Circle()
            .SetCenter(Point(center_.x, center_.y + 5 * head_r_))
            .SetRadius(2.0f * static_cast<float>(head_r_))
            .SetFillColor("rgb(240,240,240)")
            .SetStrokeColor("black"));

        container.Add(Circle()
            .SetCenter(Point(center_.x, center_.y + 2 * head_r_))
            .SetRadius(1.5f * static_cast<float>(head_r_))
            .SetFillColor("rgb(240,240,240)")
            .SetStrokeColor("black"));

        container.Add(Circle()
            .SetCenter(center_)
            .SetRadius(head_r_)
            .SetFillColor("rgb(240,240,240)")
            .SetStrokeColor("black"));
    }

private:
    Point center_;
    int head_r_;
};

// Выводит приветствие, круг и звезду
void DrawPicture() {
    Document doc;
    doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
    doc.Add(Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetFontWeight("bold"s)
                .SetData("Hello C++"s));
    doc.Add(CreateStar({20, 50}, 10, 5, 5));
    doc.Render(std::cout);
}

}  // namespace


// Выполняет линейную интерполяцию значения от from до to в зависимости от параметра t.
uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

int main() {
    using namespace svg;
    using namespace std;

    constexpr uint8_t start_r = 0;
    constexpr uint8_t end_r = 20;
    constexpr uint8_t start_g = 255;
    constexpr uint8_t end_g = 20;
    constexpr uint8_t start_b = 30;
    constexpr uint8_t end_b = 150;

    constexpr int num_circles = 10;
    Document doc;
    for (int i = 0; i < num_circles; ++i) {
        const double t = static_cast<double>(i) / (num_circles - 1);

        const string r = to_string(Lerp(start_r, end_r, t));
        const string g = to_string(Lerp(start_g, end_g, t));
        const string b = to_string(Lerp(start_b, end_b, t));

        const string fill_color = "rgb("s + r + ","s + g + ","s + b + ")"s;
        doc.Add(Circle()
                    .SetFillColor(fill_color)
                    .SetStrokeColor("black"s)
                    .SetCenter({i * 20.0 + 40, 40.0})
                    .SetRadius(15));
    }
    doc.Render(cout);
}