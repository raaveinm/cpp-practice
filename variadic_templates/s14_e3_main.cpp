#include <cassert>
#include <string>
#include <string_view>

using namespace std::literals;

template <typename H, typename... P>
bool EqualsToOneOf(const H& header, const P&... payload) {
    return ((header == payload) || ...);
}

int main() {
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
}