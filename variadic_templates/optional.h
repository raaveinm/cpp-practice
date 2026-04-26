#pragma once
#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;

    explicit Optional(const T& value) : is_initialized_(true) {
        new (data_) T(value);
    }

    explicit Optional(T&& value) : is_initialized_(true) {
        new (data_) T(std::move(value));
    }

    Optional(const Optional& other) {
        if (other.HasValue()) {
            new (data_) T(*other);
            is_initialized_ = true;
        }
    }

    Optional(Optional&& other) noexcept {
        if (other.HasValue()) {
            new (data_) T(std::move(*other));
            is_initialized_ = true;
        }
    }

    Optional& operator=(const T& value) {
        if (is_initialized_) {
            **this = value;
        } else {
            new (data_) T(value);
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(T&& rhs) {
        if (is_initialized_) {
            **this = std::move(rhs);
        } else {
            new (data_) T(std::move(rhs));
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(const Optional& rhs) {
        if (this == &rhs) return *this;
        if (!rhs.is_initialized_) {
            Reset();
        } else {
            if (is_initialized_) {
                **this = *rhs;
            } else {
                new (data_) T(*rhs);
                is_initialized_ = true;
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& rhs) noexcept {
        if (this == &rhs) return *this;
        if (!rhs.is_initialized_) {
            Reset();
        } else {
            if (is_initialized_) {
                **this = std::move(*rhs);
            } else {
                new (data_) T(std::move(*rhs));
                is_initialized_ = true;
            }
        }
        return *this;
    }

    ~Optional() { Reset(); }

    [[nodiscard]] bool HasValue() const { return is_initialized_; }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() & { return *buf; }
    const T& operator*() const & { return *buf_const; }
    T&& operator*() && { return std::move(*buf); }
    T* operator->() { return buf; }
    const T* operator->() const { return buf_const; }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() & {
        if (!is_initialized_) throw BadOptionalAccess();
        return **this;
    }
    const T& Value() const & {
        if (!is_initialized_) throw BadOptionalAccess();
        return **this;
    }
    T&& Value() && {
        if (!is_initialized_) throw BadOptionalAccess();
        return std::move(**this);
    }

    void Reset() {
        if (!is_initialized_) return;
        buf->~T();
        is_initialized_ = false;
    }

    template <typename... Args>
    void Emplace(Args&&... args) {
        Reset();
        new (data_) T(std::forward<Args>(args)...);
        is_initialized_ = true;
    }

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)]{};
    T* buf = reinterpret_cast<T*>(data_);
    const T* buf_const = reinterpret_cast<const T*>(data_);
    bool is_initialized_ = false;
};