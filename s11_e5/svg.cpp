#include "svg.h"

namespace svg {

using namespace std::literals;

    ///////////////////////////////////////////////
    /// Sub functions
    ///////////////////////////////////////////////
std::string EscapeString(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            default: result += c; break;
        }
    }
    return result;
}

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);


    context.out << std::endl;
}

    ///////////////////////////////////////////////
    /// Circle
    ///////////////////////////////////////////////
Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\""sv;
    RenderAttrs(out);
    out << "/>"sv;
}

    ///////////////////////////////////////////////
    /// Polyline
    ///////////////////////////////////////////////

Polyline & Polyline::AddPoint(const Point point) {
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext &context) const {
    auto& out = context.out;
    out << "<polyline points=\""sv;
    bool first = true;
    for (const auto& point : points_) {
        if (first) first = false;
        else out << " "sv;
        out << point.x << ","sv << point.y;
    }
    out << "\""sv;
    RenderAttrs(out);
    out << "/>"sv;
}

    ///////////////////////////////////////////////
    /// Text
    ///////////////////////////////////////////////


void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text";

    out << " x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\""sv;
    out << " dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\""sv;
    out << " font-size=\""sv << size_ << "\""sv;


    if (!font_family_.empty())
        out << " font-family=\""sv << font_family_ << "\""sv;
    if (!font_weight_.empty())
        out << " font-weight=\""sv << font_weight_ << "\""sv;

    out << ">"sv << EscapeString(data_) << "</text>"sv;
}
    ///////////////////////////////////////////////
    /// Document
    ///////////////////////////////////////////////
void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    objects_.emplace_back(std::move(obj));
}

void Document::Render(std::ostream& out) const {
    out << R"(<?xml version="1.0" encoding="UTF-8" ?>)"sv << std::endl;
    out << R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1">)"sv << std::endl;

    const RenderContext ctx(out);
    for (const auto& obj : objects_) {
        obj->Render(ctx);
    }

    out << "</svg>"sv << std::endl;
}
}  // namespace svg;