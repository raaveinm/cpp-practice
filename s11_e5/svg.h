#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <variant>

namespace svg {

class Rgb {
public:
    Rgb() = default;
    Rgb(const uint8_t red, const uint8_t green, const uint8_t blue) :
        red(red), green(green), blue(blue) {}

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

class Rgba {
public:
    Rgba() = default;
    Rgba(const uint8_t red, const uint8_t green, const uint8_t blue, const double opacity)
        : red(red)
        , green(green)
        , blue(blue)
        , opacity(opacity) {
    }

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    double opacity = 1.;
};

using Color = std::variant<std::monostate,std::string, Rgb, Rgba>;
inline const Color NoneColor{};

enum class StrokeLineCap {
    BUTT,
    ROUND,
    SQUARE,
};

enum class StrokeLineJoin {
    ARCS,
    BEVEL,
    MITER,
    MITER_CLIP,
    ROUND,
};

inline std::ostream& operator<<(std::ostream& out, StrokeLineCap stroke_line_cap) {
    switch (stroke_line_cap) {
        case StrokeLineCap::BUTT:
            return out << "butt";
        case StrokeLineCap::ROUND:
            return out << "round";
        case StrokeLineCap::SQUARE:
            return out << "square";
    }
    return out;
}

struct ColorPrinter {
    std::ostream& out;

    void operator()(std::monostate) const {
        out << "none";
    }
    void operator()(const std::string& str) const {
        out << str;
    }
    void operator()(Rgb rgb) const {
        out << "rgb(" << static_cast<int>(rgb.red) << "," << static_cast<int>(rgb.green) << "," << static_cast<int>(rgb.blue) << ")";
    }
    void operator()(Rgba rgba) const {
        out << "rgba(" << static_cast<int>(rgba.red) << "," << static_cast<int>(rgba.green) << "," << static_cast<int>(rgba.blue) << "," << rgba.opacity << ")";
    }
};

inline std::ostream& operator<<(std::ostream& out, const Color& color) {
    std::visit(ColorPrinter{out}, color);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const StrokeLineJoin stroke_line_join) {
    switch (stroke_line_join) {
        case StrokeLineJoin::ARCS:
            return out << "arcs";
        case StrokeLineJoin::BEVEL:
            return out << "bevel";
        case StrokeLineJoin::MITER:
            return out << "miter";
        case StrokeLineJoin::MITER_CLIP:
            return out << "miter-clip";
        case StrokeLineJoin::ROUND:
            return out << "round";
    }
    return out;
}

template <typename Owner>
class PathProps {
public:
    PathProps() = default;
    PathProps(Color fill_color, Color stroke_color, const int stroke_width, const StrokeLineCap line_cap,
        const StrokeLineJoin line_join)
        : fill_color_(std::move(fill_color)),
          stroke_color_(std::move(stroke_color)),
          stroke_width_(stroke_width),
          line_cap_(line_cap),
          line_join_(line_join) {
    }

    virtual ~PathProps() = default;

    Owner& SetFillColor(Color fill_color) {
        fill_color_ = std::move(fill_color);
        return static_cast<Owner&>(*this);
    }

    Owner& SetStrokeColor(Color stroke_color) {
        stroke_color_ = std::move(stroke_color);
        return static_cast<Owner&>(*this);
    }

    Owner& SetStrokeWidth(double stroke_width) {
        stroke_width_ = stroke_width;
        return static_cast<Owner&>(*this);
    }

    Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
        line_cap_ = line_cap;
        return static_cast<Owner&>(*this);
    }

    Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
        line_join_ = line_join;
        return static_cast<Owner&>(*this);
    }

protected:
    void RenderAttrs(std::ostream& out) const {
        if (fill_color_) out << " fill=\"" << *fill_color_ << "\"";
        if (stroke_color_) out << " stroke=\"" << *stroke_color_ << "\"";
        if (stroke_width_) out << " stroke-width=\"" << *stroke_width_ << "\"";
        if (line_cap_) out << " stroke-linecap=\"" << *line_cap_ << "\"";
        if (line_join_) out << " stroke-linejoin=\"" << *line_join_ << "\"";
    }

private:
    std::optional<Color> fill_color_;
    std::optional<Color> stroke_color_;
    std::optional<double> stroke_width_;
    std::optional<StrokeLineCap> line_cap_;
    std::optional<StrokeLineJoin> line_join_;
};



///////////////////////////////////////////////
/// Sub functions
///////////////////////////////////////////////
struct Point {
    Point() = default;
    Point(const double x, const double y)
        : x(x)
        , y(y) {
    }
    double x = 0;
    double y = 0;
};

/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out)
        : out(out) {
    }

    RenderContext(std::ostream& out, const int indent_step, const int indent = 0)
        : out(out)
        , indent_step(indent_step)
        , indent(indent) {
    }

    [[nodiscard]] RenderContext Indented() const {
        return {out, indent_step, indent + indent_step};
    }

    void RenderIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
public:
    void Render(const RenderContext& context) const;
    virtual ~Object() = default;

private:
    virtual void RenderObject(const RenderContext& context) const = 0;
};


class ObjectContainer {
public:
    template <typename Obj>
    void Add(Obj obj) {
        AddPtr(std::make_unique<Obj>(std::move(obj)));
    }
    virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;
    virtual ~ObjectContainer() = default;
};

class Drawable {
public:
    virtual void Draw(ObjectContainer& container) const = 0;
    virtual ~Drawable() = default;
};

    ///////////////////////////////////////////////
    /// Circle
    ///////////////////////////////////////////////
class Circle final : public Object, public PathProps<Circle>{
public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);

private:
    void RenderObject(const RenderContext& context) const override;

    Point center_;
    double radius_ = 1.0;
};

    ///////////////////////////////////////////////
    /// Polyline
    ///////////////////////////////////////////////
class Polyline : public Object, public PathProps<Polyline> {
public:
    Polyline& AddPoint(Point point);

private:
    void RenderObject(const RenderContext& context) const override;
    std::vector<Point> points_;
};

    ///////////////////////////////////////////////
    /// Text
    ///////////////////////////////////////////////
class Text : public Object, public PathProps<Text> {
public:
    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(const Point pos) {
        pos_ = pos;
        return *this;
    }

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(const Point offset) {
        offset_ = offset;
        return *this;
    }

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontSize(uint32_t size) {
        size_ = size;
        return *this;
    }

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family) {
        font_family_ = std::move(font_family);
        return *this;
    }

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight) {
        font_weight_ = std::move(font_weight);
        return *this;
    }

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data) {
        data_ = std::move(data);
        return *this;
    }


private:
    void RenderObject(const RenderContext& context) const override;

    Point pos_;
    Point offset_;
    uint32_t size_ = 1;
    std::string font_family_;
    std::string font_weight_;
    std::string data_;
};

    ///////////////////////////////////////////////
    /// Document
    ///////////////////////////////////////////////
class Document : public ObjectContainer {
public:
    /*
     Метод Add добавляет в svg-документ любой объект-наследник svg::Object.
     Пример использования:
     Document doc;
     doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
    */

    // Добавляет в svg-документ объект-наследник svg::Object
    void AddPtr(std::unique_ptr<Object>&& obj) override;

    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;

private:
    std::vector<std::unique_ptr<Object>> objects_;

};

}  // namespace svg