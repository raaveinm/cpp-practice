#include "ppm_image.h"

#include <iostream>
#include <string_view>
#include <cstddef>

using namespace std;

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    // Инвертируем изображение row-by-row
    for (int y = 0; y < image.GetHeight(); ++y) {
        img_lib::Color* line = image.GetLine(y);
        for (int x = 0; x < image.GetWidth(); ++x) {
            line[x].r = std::byte(255 - std::to_integer<int>(line[x].r));
            line[x].g = std::byte(255 - std::to_integer<int>(line[x].g));
            line[x].b = std::byte(255 - std::to_integer<int>(line[x].b));
        }
    }

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
