#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
#include "json.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(
        spendings.begin(), spendings.end(), 0, [](int current, const Spending& spending){
            return current + spending.amount;
        });
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
    const json::Document document = json::Load(input);
    const json::Node& root = document.GetRoot();
    vector<json::Node> nodes = root.AsArray();
    vector<Spending> spending_vector;
    spending_vector.reserve(nodes.size());
    for (const json::Node& node : nodes) {
        spending_vector.push_back(
            {node.AsMap().at("category").AsString(), node.AsMap().at("amount").AsInt()}
        );
    }
    return spending_vector;
}

int main() {
    // не меняйте main
    const vector<Spending> spendings = LoadFromJson(cin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}