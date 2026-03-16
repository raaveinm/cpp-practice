#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct BookingData {
    int64_t time;
    string hotel_name;
    int client_id;
    int room_count;
};

class HotelManager {
public:
    void Book(const int64_t time, const string& hotel_name, const int client_id, const int room_count) {
        current_time_ = time;
        history_.push({time, hotel_name, client_id, room_count});
        rooms_[hotel_name] += room_count;
        clients_[hotel_name][client_id]++;
        AutoCleanup();
    }

    int ComputeClientCount(const std::string& hotel_name) {
        if (!clients_.contains(hotel_name)) return 0;
        return clients_[hotel_name].size();
    }

    int ComputeRoomCount(const std::string& hotel_name) {
        if (!rooms_.contains(hotel_name)) return 0;
        return rooms_[hotel_name];
    }

private:
    static constexpr int DAY_SECONDS = 86400;
    std::queue<BookingData> history_;
    std::unordered_map<std::string, int64_t> rooms_;
    std::unordered_map<std::string, std::unordered_map<int, int>> clients_;
    int64_t current_time_ = 0;

    void AutoCleanup() {
        while (!history_.empty() && current_time_ - history_.front().time >= DAY_SECONDS) {
            const BookingData& booking = history_.front();
            auto& hotel_clients = clients_[booking.hotel_name];
            rooms_[booking.hotel_name] -= booking.room_count;

            if (--hotel_clients[booking.client_id] == 0)
                hotel_clients.erase(booking.client_id);

            history_.pop();
        }
    }
};

int main() {
    // Crucial for performance with 10^5 queries
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    HotelManager manager;
    int query_count;
    std::cin >> query_count;

    for (int i = 0; i < query_count; ++i) {
        std::string command;
        std::cin >> command;

        if (command == "BOOK") {
            int64_t time;
            std::string hotel_name;
            int client_id, room_count;
            std::cin >> time >> hotel_name >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        } else if (command == "CLIENTS") {
            std::string hotel_name;
            std::cin >> hotel_name;
            std::cout << manager.ComputeClientCount(hotel_name) << "\n";
        } else if (command == "ROOMS") {
            std::string hotel_name;
            std::cin >> hotel_name;
            std::cout << manager.ComputeRoomCount(hotel_name) << "\n";
        }
    }
    return 0;
}