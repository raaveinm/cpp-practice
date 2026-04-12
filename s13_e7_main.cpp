#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <variant>
#include <vector>

using namespace std;

// напишите функцию ComputeStatistics, принимающую 5 параметров:
// два итератора, выходное значение для суммы, суммы квадратов и максимального элемента

struct OnlySum {
    int value;
};

OnlySum operator+(OnlySum l, OnlySum r) {
    return {l.value + r.value};
}
OnlySum& operator+=(OnlySum& l, OnlySum r) {
    return l = l + r;
}

template <typename InputIt, typename OutSum, typename OutSqSum, typename OutMax>
void ComputeStatistics(InputIt begin, InputIt end, OutSum& out_sum, OutSqSum& out_sq_sum, OutMax& out_max) {
    bool first = true;
    for (auto it = begin; it != end; ++it) {
        auto val = *it;
        if constexpr (!is_same_v<decay_t<OutSum>, nullopt_t>) {
            if constexpr (is_same_v<decay_t<OutSum>, optional<decay_t<decltype(val)>>>) {
                if (first) out_sum = val;
                else *out_sum += val;
            } else {
                if (first) out_sum = val;
                else out_sum += val;
            }
        }
        if constexpr (!is_same_v<decay_t<OutSqSum>, nullopt_t>) {
            if constexpr (is_same_v<decay_t<OutSqSum>, optional<decay_t<decltype(val * val)>>>) {
                if (first) out_sq_sum = val * val;
                else *out_sq_sum += val * val;
            } else {
                if (first) out_sq_sum = val * val;
                else out_sq_sum += val * val;
            }
        }
        if constexpr (!is_same_v<decay_t<OutMax>, nullopt_t>) {
            if constexpr (is_same_v<decay_t<OutMax>, optional<int>>) {
                if (first || val > *out_max) out_max = val;
            } else {
                if (first || val > out_max) out_max = val;
            }
        }
        first = false;
    }
}

int main() {
    vector input = {1, 2, 3, 4, 5, 6};
    int sq_sum;
    std::optional<int> max;

    // Переданы выходные параметры разных типов - std::nullopt_t, int и std::optional<int>
    ComputeStatistics(input.begin(), input.end(), nullopt, sq_sum, max);

    assert(sq_sum == 91 && max && *max == 6);

    vector<OnlySum> only_sum_vector = {{100}, {-100}, {20}};
    OnlySum sum;

    // Поданы значения поддерживающие только суммирование, но запрошена только сумма
    ComputeStatistics(only_sum_vector.begin(), only_sum_vector.end(), sum, nullopt, nullopt);

    assert(sum.value == 20);
}
