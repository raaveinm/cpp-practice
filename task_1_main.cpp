//
// Created by Kirill "Raaveinm" on 12/24/25.
//

#include <cassert>
#include <string>
#include <set>
#include <map>
#include <string_view>

using namespace std;

class Translator {
public:
    Translator() = default;
    ~Translator() = default;

    void Add(const std::string_view source, const std::string_view target) {
        const std::string& source_ref = *data_.insert(std::string(source)).first;
        const std::string& target_ref = *data_.insert(std::string(target)).first;
        forward_[source_ref] = target_ref;
        backward_[target_ref] = source_ref;}

    [[nodiscard]] std::string_view TranslateForward(const std::string_view source) const {
        if (const auto it = forward_.find(source); it != forward_.end()) {
            return it->second;
        }
        return {};
    }

    [[nodiscard]] std::string_view TranslateBackward(const std::string_view target) const {
        if (const auto it = backward_.find(target); it != backward_.end()) {
            return it->second;
        }
        return {};
    }
private:
    std::set<std::string> data_;
    std::map<std::string_view, std::string_view> forward_;
    std::map<std::string_view, std::string_view> backward_;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s).empty());
}

int main() {
    TestSimple();
}