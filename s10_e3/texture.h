#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    // Возвращает размер изображения
    [[nodiscard]] Size GetSize() const {
        return GetImageSize(image_);
    }

    // Возвращает цвет пикселя.
    // Если координаты выходят за границы изображения, возвращается пробел
    char GetPixelColor(Point p) const {
        if (p.y < 0 || p.y >= static_cast<int>(image_.size())) {
            return ' ';
        }
        if (p.x < 0 || p.x >= static_cast<int>(image_[p.y].size())) {
            return ' ';
        }
        return image_[p.y][p.x];
    }

private:
    Image image_;
};