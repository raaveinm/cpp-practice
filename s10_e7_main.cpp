#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;


// clang-format off

constexpr int NUM_PLANETS = 8;
constexpr string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

// clang-format on

bool IsPlanet(const string_view name) {
    const auto it = std::ranges::find(PLANETS, name);
    return it != PLANETS + NUM_PLANETS;
}

void Test(const string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv)
        << "a planet"sv << endl;
}

void ReadName(istream& instream) {
    string name;
    std::getline(instream, name);
    Test(name);
}

int main() {
    Test("Earth"sv);
    Test("Jupiter"sv);
    Test("Pluto"sv);
    Test("Moon"sv);
    ReadName(cin);
    return 0;
}