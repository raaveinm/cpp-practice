//
// Created by raaveinm on 1/20/26.
//

#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct DumbbellHash {
    size_t operator()(const Dumbbell& dumbbell) const {
        size_t res = 0;
        size_t pos = 1;
        auto add = [&](size_t v) {
            res += v * pos;
            pos *= 37;
        };
        add(hash<double>()(dumbbell.circle1.x));
        add(hash<double>()(dumbbell.circle1.y));
        add(hash<double>()(dumbbell.circle1.r));
        add(hash<double>()(dumbbell.circle2.x));
        add(hash<double>()(dumbbell.circle2.y));
        add(hash<double>()(dumbbell.circle2.r));
        add(hash<string>()(dumbbell.text));
        return res;
    }
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{{10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s};
    cout << "Dumbbell hash "s << hash(dumbbell);
}