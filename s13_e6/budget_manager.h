#pragma once

#include "date.h"

#include <vector>

class BudgetManager {
public:
    static const Date START_DATE;
    static const Date END_DATE;

    [[nodiscard]] double ComputeIncome(const Date& from, const Date& to) const;
    void Earn(const Date& from, const Date& to, int income);
    void PayTax(const Date& from, const Date& to, int percent);
    void Spend(const Date& from, const Date& to, int amount);

private:
    std::vector<double> daily_income_ = std::vector<double>(static_cast<size_t>(Date::ComputeDistance(START_DATE, END_DATE) + 1), 0.0);
    std::vector<double> daily_spent_ = std::vector<double>(static_cast<size_t>(Date::ComputeDistance(START_DATE, END_DATE) + 1), 0.0);
    [[nodiscard]] static int GetDayIndex(const Date& date) { return Date::ComputeDistance(START_DATE, date); }
};
