#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>


class Date {
public:
    explicit Date(const std::string& str) {
        std::istringstream input(str);
        std::tm tm = {};
        if (input >> std::get_time(&tm, "%Y-%m-%d"))
            ymd_ = std::chrono::year(tm.tm_year + 1900) / (tm.tm_mon + 1) / tm.tm_mday;
    }

    explicit Date(std::string_view str)
        : Date(std::string(str)) {
    }

    static int ComputeDistance(const Date& from, const Date& to) {
        return (std::chrono::sys_days(to.ymd_) - std::chrono::sys_days(from.ymd_)).count();
    }

private:
    std::chrono::year_month_day ymd_{};
};