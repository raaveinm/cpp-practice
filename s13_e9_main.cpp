#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "assets/log_duration.h"

using namespace std;

class RandomContainer {
public:
    void Insert(const int val) {
        if (Has(val)) return;
        val_to_index_[val] = values_pool_.size();
        values_pool_.push_back(val);
    }
    void Remove(const int val) {
        if (!Has(val)) return;
        const int index = val_to_index_[val];
        const int last_val = values_pool_.back();

        values_pool_[index] = last_val;
        val_to_index_[last_val] = index;

        values_pool_.pop_back();
        val_to_index_.erase(val);
    }
    bool Has(const int val) const {
        return val_to_index_.contains(val);
    }
    int GetRand() const {
        uniform_int_distribution<> distr(0, values_pool_.size() - 1);
        const int rand_index = distr(engine_);
        return values_pool_.at(rand_index);
    }

private:
    vector<int> values_pool_;
    unordered_map<int, int> val_to_index_;
    mutable mt19937 engine_;
};

int main() {
    RandomContainer container;
    int query_num = 0;
    cin >> query_num;
    {
        LOG_DURATION("Requests handling"s);
        for (int query_id = 0; query_id < query_num; query_id++) {
            string query_type;
            cin >> query_type;
            if (query_type == "INSERT"s) {
                int value = 0;
                cin >> value;
                container.Insert(value);
            } else if (query_type == "REMOVE"s) {
                int value = 0;
                cin >> value;
                container.Remove(value);
            } else if (query_type == "HAS"s) {
                int value = 0;
                cin >> value;
                if (container.Has(value)) {
                    cout << "true"s << endl;
                } else {
                    cout << "false"s << endl;
                }
            } else if (query_type == "RAND"s) {
                cout << container.GetRand() << endl;
            }
        }
    }
}
