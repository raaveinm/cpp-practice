#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;



class House {
public:
    House(const int length, const int width, const int height)
    : length_(length), width_(width), height_(height){}

    [[nodiscard]] int GetLength() const { return length_; }
    [[nodiscard]] int GetWidth() const { return width_; }
    [[nodiscard]] int GetHeight() const { return height_; }

private:
    int length_;
    int width_;
    int height_;
};

class Resources {
public:
    explicit Resources(const int brick_count) : brick_count_(brick_count) {}

    [[nodiscard]] int GetBrickCount() const { return brick_count_; }

    void TakeBricks(const int amount) {
        if (amount < 0 || amount > brick_count_) {
            throw std::out_of_range("influence amount");
        }
        brick_count_ -= amount;
    }

private:
    int brick_count_;
};

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class Builder {
public:
    explicit Builder(Resources& resources) : resources_(resources) {}

    House BuildHouse(const HouseSpecification spec) const {
        const int bricks_needed = 2 * (spec.length + spec.width) * spec.height * 32;

        try {
            resources_.TakeBricks(bricks_needed);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Failed to build house: not enough bricks");
        }

        return {spec.length, spec.width, spec.height};
    }

private:
    Resources& resources_;
};

int main() {
    Resources resources{10000};
    Builder builder1{resources};
    Builder builder2{resources};

    const House house1 = builder1.BuildHouse(HouseSpecification{12, 9, 3});
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    const House house2 = builder2.BuildHouse(HouseSpecification{8, 6, 3});
    assert(house2.GetLength() == 8);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    const House banya = builder1.BuildHouse(HouseSpecification{4, 3, 2});
    assert(banya.GetHeight() == 2);
    cout << resources.GetBrickCount() << " bricks left"s << endl;
}