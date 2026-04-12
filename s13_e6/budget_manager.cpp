#include "budget_manager.h"

#include <ranges>
#include "date.h"

using namespace std::literals;

const Date BudgetManager::START_DATE = Date("2000-01-01"s);
const Date BudgetManager::END_DATE = Date("2100-01-01"s);

double BudgetManager::ComputeIncome(const Date &from, const Date &to) const {
    double total(0.0);
    for (const int i : std::ranges::views::iota(GetDayIndex(from), GetDayIndex(to) + 1)) {
        total += daily_income_[i] - daily_spent_[i];
    }
    return total;
}

void BudgetManager::Earn(const Date &from, const Date &to, const int income) {
    const int days = Date::ComputeDistance(from, to) + 1;
    const double daily = static_cast<double>(income) / days;

    for (const int i : std::ranges::views::iota(GetDayIndex(from), GetDayIndex(to) + 1)) {
        daily_income_[i] += daily;
    }
}

void BudgetManager::PayTax(const Date &from, const Date &to, const int percent) {
    for (const int i : std::ranges::views::iota(GetDayIndex(from), GetDayIndex(to) + 1) ) {
        daily_income_[i] *= 1.0 - static_cast<double>(percent) / 100.0;
    }
}

void BudgetManager::Spend(const Date &from, const Date &to, const int amount) {
    const int days = Date::ComputeDistance(from, to) + 1;
    const double daily = static_cast<double>(amount) / days;
    for (const int i : std::ranges::views::iota(GetDayIndex(from), GetDayIndex(to) + 1)) {
        daily_spent_[i] += daily;
    }
}
