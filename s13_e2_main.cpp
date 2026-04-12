//
// Created by Kirill "Raaveinm" on 3/28/26.
//

#include <iomanip>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>
#include <string>


class ReadManager {
public:
    ReadManager() : user_page_(MAX_USER_COUNT_ + 1, 0), pages_stat_(MAX_PAGE_COUNT_ + 1, 0) {}

    [[nodiscard]] double Cheer(const int usr) const {
        if (user_page_[usr] == 0) return 0;
        if (user_count_ == 1) return 1;

        const int usr_page = user_page_[usr];
        int amount = 0;
        for (int p = 1; p < usr_page; ++p) {
            amount += pages_stat_[p];
        }

        return static_cast<double>(amount) / static_cast<double>(user_count_ - 1);
    }

    void Read(const int user, const int page) {
        if (user_page_[user] == 0) {
            user_count_++;
        } else {
            pages_stat_[user_page_[user]]--;
        }
        user_page_[user] = page;
        pages_stat_[page]++;
    }

private:
    static constexpr int MAX_USER_COUNT_ = 100000;
    static constexpr int MAX_PAGE_COUNT_ = 1000;
    std::vector<int> user_page_;
    std::vector<int> pages_stat_;
    int user_count_ = 0;
};

void CommandManager(ReadManager& manager, std::istream& input, std::ostream& out) {
    int amount_of_commands;
    input >> amount_of_commands;
    for (int i = 0; i < amount_of_commands; ++i) {
        std::string command;
        input >> command;

        if (command == "CHEER") {
            int usr;
            input >> usr;
            out << std::setprecision(6) << manager.Cheer(usr) << std::endl;
        } else if (command == "READ") {
            int usr, page;
            input >> usr >> page;
            manager.Read(usr, page);
        }
    }
}

int main() {
    ReadManager manager;
    CommandManager(manager, std::cin, std::cout);
    return 0;
}