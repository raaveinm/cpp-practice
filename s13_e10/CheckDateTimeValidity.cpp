#include <array>
#include <stdexcept>
#include <string>

// dummies
using namespace std::literals;

struct DateTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

void CheckDateTimeValidity(const DateTime& dt) {
    switch (dt.year < 1 ? -1 : (dt.year > 9999 ? 1 : 0)) {
        case -1: throw std::domain_error("year is too small"s);
        case 1:  throw std::domain_error("year is too big"s);
        default: break;
    }

    switch (dt.month < 1 ? -1 : (dt.month > 12 ? 1 : 0)) {
        case -1: throw std::domain_error("month is too small"s);
        case 1:  throw std::domain_error("month is too big"s);
        default: break;
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const std::array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    switch (dt.day < 1 ? -1 : (dt.day > month_lengths[dt.month - 1] ? 1 : 0)) {
        case -1: throw std::domain_error("day is too small"s);
        case 1:  throw std::domain_error("day is too big"s);
        default: break;
    }

    switch (dt.hour < 0 ? -1 : (dt.hour > 23 ? 1 : 0)) {
        case -1: throw std::domain_error("hour is too small"s);
        case 1:  throw std::domain_error("hour is too big"s);
        default: break;
    }

    switch (dt.minute < 0 ? -1 : (dt.minute > 59 ? 1 : 0)) {
        case -1: throw std::domain_error("minute is too small"s);
        case 1:  throw std::domain_error("minute is too big"s);
        default: break;
    }

    switch (dt.second < 0 ? -1 : (dt.second > 59 ? 1 : 0)) {
        case -1: throw std::domain_error("second is too small"s);
        case 1:  throw std::domain_error("second is too big"s);
        default: break;
    }
}
