#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) : size_(a_size) {
        if (size_ > N) {
            throw std::invalid_argument("StackVector size cannot be larger than capacity");
        }
    }

    T& operator[](size_t index) {
        return array_vector_[index];
    }
    const T& operator[](size_t index) const {
        return array_vector_[index];
    }

    auto begin() { return array_vector_.begin(); }
    auto end() { return array_vector_.begin() + size_; }
    auto begin() const { return array_vector_.begin(); }
    auto end() const { return array_vector_.begin() + size_; }

    [[nodiscard]] size_t Size() const { return size_; }
    [[nodiscard]] static size_t Capacity() { return N; }

    void PushBack(const T& value) {
        if (size_ == N) {
            throw std::overflow_error("Overflow");
        }
        array_vector_[size_] = value;
        ++size_;
    }

    T PopBack() {
        if (size_ == 0) {
            throw std::underflow_error("Underflow");
        }
        --size_;
        return array_vector_[size_];
    }

private:
    std::array<T, N> array_vector_;
    size_t size_ = 0;
};
