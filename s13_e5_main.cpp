#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;


///////////////////////////////////////////////
// Domain class
///////////////////////////////////////////////

class Domain {
public:
    explicit Domain(std::string domain) : domain_(std::move(domain)) {
        std::ranges::reverse(domain_);
        domain_.push_back('.');
    }

    [[nodiscard]] bool IsSubdomain(const Domain& other) const {
        return domain_.starts_with(other.domain_);
    }
    auto operator<=>(const Domain& other) const {
        return domain_ <=> other.domain_;
    }

    bool operator==(const Domain & d2) const = default;

private:
    std::string domain_;
};

template <typename Iterator>
class DomainChecker {
public:
    explicit DomainChecker(Iterator begin, Iterator end) {
        forbidden_domains_.assign(begin, end);
        std::ranges::sort(forbidden_domains_, std::less{});
        auto last = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
                                [](const Domain& lhs, const Domain& rhs) {
                                    return rhs.IsSubdomain(lhs);
                                });
        forbidden_domains_.erase(last, forbidden_domains_.end());
    }

    [[nodiscard]] bool IsForbidden(const Domain& domain) const {
        const auto it = ranges::upper_bound(forbidden_domains_, domain);
        if (it == forbidden_domains_.begin())
            return false;
        return domain.IsSubdomain(*std::prev(it));
    }

private:
    std::vector<Domain> forbidden_domains_;

};


///////////////////////////////////////////////
// Readers
///////////////////////////////////////////////

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

vector<Domain> ReadDomains(istream& input, size_t count) {
    vector<Domain> domains;
    for (size_t i = 0; i < count; ++i) {
        string domain;
        getline(input, domain);
        domains.emplace_back(domain);
    }
    return domains;
}


///////////////////////////////////////////////
// Tests
///////////////////////////////////////////////

void TestDomainMethods() {
    const Domain d1("epicgames.com");
    const Domain d2("epicgames.com");
    const Domain sub_d1("math.epicgames.com");
    const Domain fake_sub("freeepicgames.com");
    const Domain super_sub_d1("history.math.epicgames.com");

    assert(d1 == d2);
    assert(sub_d1.IsSubdomain(d1));
    assert(super_sub_d1.IsSubdomain(d1));
    assert(!d1.IsSubdomain(sub_d1));
}

void TestReadDomains() {
    std::istringstream input("epicgames.com"
                             "\nmath.epicgames.com"
                             "\nsteampowered.com\n");
    const std::vector<Domain> domains = ReadDomains(input, 3);

    assert(domains.size() == 3);
    assert(domains[0] == Domain("epicgames.com"));
    assert(domains[1] == Domain("math.epicgames.com"));
    assert(domains[2] == Domain("steampowered.com"));
}

void TestDomainChecker() {
    std::vector forbidden = {
        Domain("fake.epicgames.com"),
        Domain("steampowered.ru"),
        Domain("cn")
    };

    DomainChecker checker(forbidden.begin(), forbidden.end());

    // forbidden
    assert(checker.IsForbidden(Domain("fake.epicgames.com")));
    assert(checker.IsForbidden(Domain("store.fake.epicgames.com")));
    assert(checker.IsForbidden(Domain("store.steampowered.ru")));
    assert(checker.IsForbidden(Domain("cn")));
    assert(checker.IsForbidden(Domain("gov.cn")));

    // allowed
    assert(!checker.IsForbidden(Domain("steampowered.com")));
    assert(!checker.IsForbidden(Domain("epicgames.com")));
    assert(!checker.IsForbidden(Domain("en")));
}

int main() {
    TestDomainMethods();
    TestReadDomains();
    TestDomainChecker();
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    const DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
