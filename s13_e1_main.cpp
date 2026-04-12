#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Gender { FEMALE, MALE };

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = range_copy.begin() + range_copy.size() / 2;
    nth_element(range_copy.begin(), middle, range_copy.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return middle->age;
}

// напишите сигнатуру и реализацию функции PrintStats
void PrintStats(vector<Person>& p) {
    const auto female_end = partition(p.begin(), p.end(), [](const Person& person) {
        return person.gender == Gender::FEMALE;
    });
    const auto employed_females_end = partition(p.begin(), female_end, [](const Person& person) {
        return person.is_employed;
    });
    const auto employed_males_end = partition(female_end, p.end(), [](const Person& person) {
        return person.is_employed;
    });

    cout << "Median age = "s << ComputeMedianAge(p.begin(), p.end()) << endl;
    cout << "Median age for females = "s << ComputeMedianAge(p.begin(), female_end) << endl;
    cout << "Median age for males = "s << ComputeMedianAge(female_end, p.end()) << endl;
    cout << "Median age for employed females = "s << ComputeMedianAge(p.begin(), employed_females_end) << endl;
    cout << "Median age for unemployed females = "s << ComputeMedianAge(employed_females_end, female_end) << endl;
    cout << "Median age for employed males = "s << ComputeMedianAge(female_end, employed_males_end) << endl;
    cout << "Median age for unemployed males = "s << ComputeMedianAge(employed_males_end, p.end()) << endl;
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},   {40, Gender::FEMALE, true},  {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},  {80, Gender::FEMALE, false}, {78, Gender::MALE, false},
        {10, Gender::FEMALE, false}, {55, Gender::MALE, true},
    };
    PrintStats(persons);
}