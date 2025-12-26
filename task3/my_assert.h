#pragma once
#include <memory>
#include <string>

namespace detail {

class AssertionFailed final {
public:
    AssertionFailed(std::string message, std::string file, std::string func, unsigned line)
        : message_{std::move(message)}
        , file_{std::move(file)}
        , function_{std::move(func)}
        , line_(line) {
    }

    [[nodiscard]] const std::string& GetMessage() const {
        return message_;
    }

    [[nodiscard]] const std::string& GetFile() const {
        return file_;
    }

    [[nodiscard]] const std::string& GetFunction() const {
        return function_;
    }

    [[nodiscard]] unsigned GetLine() const {
        return line_;
    }

private:
    std::string message_;
    std::string file_;
    std::string function_;
    unsigned line_;
};

inline void AssertImpl(const char* message, char const* file, char const* func, unsigned line) {
    throw AssertionFailed(message, file, func, line);
}

}  // namespace detail

#ifdef assert
#undef assert
#endif

#define assert(expression)   \
    (void) ((!!(expression)) \
    || (::detail::AssertImpl(#expression, __FILE__, __FUNCTION__, static_cast<unsigned>(__LINE__)), 0))

#define EOF_GUARD