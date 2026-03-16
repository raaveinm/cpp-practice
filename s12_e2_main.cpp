#include <cassert>
#include <functional>
#include <optional>
#include <string>

using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init) : init_(move(init)) {}

    bool HasValue() const {
        return has_value_;
    }

    const T& Get() const {
        if (!has_value_) {
            val_ = init_();
            has_value_ = true;
        }
        return *val_;
    }

private:
    mutable std::optional<T> val_;
    function<T()> init_;
    mutable bool has_value_ = false;
};

void UseExample() {
    const auto big_string = "Giant amounts of memory"s;

    LazyValue<string> lazy_string([&big_string] {
        return big_string;
    });

    assert(!lazy_string.HasValue());
    assert(lazy_string.Get() == big_string);
    assert(lazy_string.Get() == big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    assert(!called);
}

int main() {
    UseExample();
    TestInitializerIsntCalled();
}