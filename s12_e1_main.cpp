#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Person {
    string name;
    int age{}, income{};
    bool is_male{};
};

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (auto& [name, age, income, is_male] : result) {
        char gender;
        input >> name >> age >> income >> gender;
        is_male = (gender == 'M');
    }

    return result;
}

int main() {
    vector<Person> people = ReadPeople(cin);

    vector<int> ages;
    ages.reserve(people.size());
    for (const auto& p : people) ages.push_back(p.age);
    ranges::sort(ages);

    vector<int> incomes;
    incomes.reserve(people.size());
    for (const auto& p : people) incomes.push_back(p.income);
    ranges::sort(incomes, greater<int>{});

    vector<long long> prefix_incomes(incomes.size() + 1, 0);
    for (size_t i = 0; i < incomes.size(); ++i) {
        prefix_incomes[i + 1] = prefix_incomes[i] + incomes[i];
    }

    auto get_popular_name = [&](char gender) -> string {
        map<string, int> counts;
        for (const auto& p : people) {
            if ((gender == 'M' && p.is_male) || (gender == 'W' && !p.is_male)) {
                counts[p.name]++;
            }
        }
        if (counts.empty()) return "";

        string best_name;
        int max_count = 0;
        for (const auto& [name, count] : counts) {
            if (count > max_count) {
                max_count = count;
                best_name = name;
            }
        }
        return best_name;
    };

    string pop_m = get_popular_name('M');
    string pop_w = get_popular_name('W');

    for (string command; cin >> command;) {
        if (command == "AGE"s) {
            int adult_age;
            cin >> adult_age;
            const auto it = ranges::lower_bound(ages, adult_age);
            cout << "There are "s << distance(it, ages.end())
                 << " adult people for maturity age "s << adult_age << '\n';

        } else if (command == "WEALTHY"s) {
            int count;
            cin >> count;
            count = min(count, static_cast<int>(people.size()));
            cout << "Top-"s << count << " people have total income "s
                 << prefix_incomes[count] << '\n';

        } else if (command == "POPULAR_NAME"s) {
            char gender;
            cin >> gender;
            const string& best_name = (gender == 'M') ? pop_m : pop_w;

            if (best_name.empty()) {
                cout << "No people of gender "s << gender << '\n';
            } else {
                cout << "Most popular name among people of gender "s << gender
                     << " is "s << best_name << '\n';
            }
        }
    }
    return 0;
}