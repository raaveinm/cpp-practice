#include <iostream>
#include <string>

#include "budget_manager.h"
#include "date.h"

void ParseAndProcessQuery(BudgetManager& manager, std::string_view line) {
    if (line.starts_with("ComputeIncome")) {
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date from{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date to{line.substr(0, line.find(' '))};
        std::cout << manager.ComputeIncome(from, to) << '\n';

    } else if (line.starts_with("Earn")) {
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date from{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date to{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));

        const int income = std::stoi(std::string(line));
        manager.Earn(from, to, income);

    } else if (line.starts_with("PayTax")) {
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date from{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date to{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const int percent = std::stoi(std::string(line));
        manager.PayTax(from, to, percent);


    } else if (line.starts_with("Spend")) {
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date from{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const Date to{line.substr(0, line.find(' '))};
        line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
        const int amount = std::stoi(std::string(line));
        manager.Spend(from, to, amount);
    }
}

int ReadNumberOnLine(std::istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}