#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE};

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(const ShapeType type) : type_(type) {}

    void SetPosition(const Point pos) {
        pos_ = pos;
    }

    void SetSize(const Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = std::move(texture);
    }

    // Рисует фигуру на указанном изображении
    // В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        for (int y = 0; y < size_.height; ++y) {
            for (int x = 0; x < size_.width; ++x) {
                const int x_ = x + pos_.x;
                const int y_ = y + pos_.y;

                if (y_ < 0 || y_ >= static_cast<int>(image.size()) ||
                    x_ < 0 || x_ >= static_cast<int>(image[y_].size()))
                    continue;

                if (type_ == ShapeType::ELLIPSE && !IsPointInEllipse({x, y}, size_)) {
                    continue;
                }

                char color = '.';
                if (texture_) {
                    Size t_size = texture_->GetSize();
                    if (x < t_size.width && y < t_size.height) {
                        color = texture_->GetPixelColor({x, y});
                    }
                }
                image[y_][x_] = color;
            }
        }
    }

private:
    ShapeType type_;
    Point pos_{0, 0};
    Size size_{0, 0};
    std::shared_ptr<Texture> texture_;
};