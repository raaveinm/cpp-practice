#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

using namespace std;

class RouteManager {
public:
    void AddRoute(const int start, const int finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }
    [[nodiscard]] int FindNearestFinish(const int start, int finish) const {
        int result = abs(start - finish);
        if (!reachable_lists_.contains(start)) return result;

        const auto& s = reachable_lists_.at(start);
        auto it = s.lower_bound(finish);

        if (it != s.end()) {
            result = min(result, abs(*it - finish));
        }
        if (it != s.begin()) {
            result = min(result, abs(*prev(it) - finish));
        }

        return result;
    }

private:
    unordered_map<int, set<int>> reachable_lists_;
};

int main() {
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD"s) {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO"s) {
            cout << routes.FindNearestFinish(start, finish) << "\n"s;
        }
    }
}