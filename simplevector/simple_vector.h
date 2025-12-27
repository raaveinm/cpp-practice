//
// Created by Kirill "Raaveinm" on 12/27/25.
//

#ifndef TMP_SIMPLEVECTOR_H
#define TMP_SIMPLEVECTOR_H

#include <cstddef>
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

struct ReserveProxy { size_t capacity; };
inline ReserveProxy Reserve(const size_t capacity) { return ReserveProxy(capacity);}

template <typename Type>
class SimpleVector {
public:
    ///////////////////////////////////////////////
    // Constructor & Destructor
    ///////////////////////////////////////////////
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept : items_(nullptr), size_(0), capacity_(0) {}

    explicit SimpleVector(const size_t size) {
        if (size == 0) {
            items_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            return;
        }
        items_ = new Type[size];
        std::fill(items_, items_ + size, Type());
        size_ = size;
        capacity_ = size;
    }

    SimpleVector(size_t size, const Type& value) {
        items_ = new Type[size];
        std::fill(items_, items_ + size, value);
        size_ = size;
        capacity_ = size;
    }

    SimpleVector(std::initializer_list<Type> init) {
        items_ = new Type[init.size()];
        std::move(init.begin(), init.end(), items_);
        size_ = init.size();
        capacity_ = init.size();
    }

    SimpleVector(const SimpleVector& other) {
        if (other.capacity_ == 0) {
            items_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        } else {
            items_ = new Type[other.capacity_];
            std::move(other.begin(), other.end(), items_);
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
    }

    explicit SimpleVector(const ReserveProxy proxy) {
        items_ = new Type[proxy.capacity];
        size_ = 0;
        capacity_ = proxy.capacity;
    }

    ~SimpleVector() {
        delete[] items_;
    }

    ///////////////////////////////////////////////
    // Element Access
    ///////////////////////////////////////////////
    [[nodiscard]] size_t GetSize() const noexcept { return size_; }
    [[nodiscard]] size_t GetCapacity() const noexcept { return capacity_; }
    [[nodiscard]] bool IsEmpty() const noexcept { return size_ == 0; }

    ///////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////////////
    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return items_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return items_[index];
    }

    Type& At(size_t index) {
        if (index >= size_)
            throw std::out_of_range("Index out of range");
        return items_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_)
            throw std::out_of_range("Index out of range");
        return items_[index];
    }

    ///////////////////////////////////////////////
    // Iterators
    ///////////////////////////////////////////////
    Iterator begin() noexcept { return items_; }
    Iterator end() noexcept { return items_ + size_; }
    ConstIterator begin() const noexcept { return items_; }
    ConstIterator end() const noexcept { return items_ + size_; }
    ConstIterator cbegin() const noexcept { return items_; }
    ConstIterator cend() const noexcept { return items_ + size_; }

    ///////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////
    void Clear() noexcept { size_ = 0; }

    // Reserve vector capacity
    void Reserve(const size_t new_capacity) {
        if (new_capacity > capacity_) {
            Type* new_items = new Type[new_capacity];
            std::move(items_, items_ + size_, new_items);
            delete[] items_;
            items_ = new_items;
            capacity_ = new_capacity;
        }
    }

    // Resize vector size
    void Resize(const size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
        } else if (new_size <= capacity_) {
            std::fill(items_ + size_, items_ + new_size, Type());
            size_ = new_size;
        } else {
            Reserve(std::max(new_size, capacity_ * 2));
            std::fill(items_ + size_, items_ + new_size, Type());
            size_ = new_size;
        }
    }

    ///////////////////////////////////////////////
    // Data Manipulation
    ///////////////////////////////////////////////

    // Insert new element to the end of vector
    void PushBack(const Type& value) {
        if (size_ == capacity_) {
            Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        items_[size_] = value;
        ++size_;
    }

    // Clear last element
    void PopBack() noexcept {
        if (size_ > 0)
            --size_;
    }

    // Remove element from vector
    Iterator Erase(ConstIterator it) {
        auto pos = const_cast<Iterator>(it);
        std::move(pos + 1, end(), pos);
        --size_;
        return pos;
    }

    // Insert new element to vector
    Iterator Insert(ConstIterator it, const Type& value) {
        size_t index = it - begin();
        if (size_ == capacity_) {
            Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        std::move_backward(begin() + index, end(), end() + 1);
        items_[index] = value;
        ++size_;
        return items_ + index;
    }

    void swap(SimpleVector& other) noexcept {
        std::swap(items_, other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this == &rhs)
            return *this;

        SimpleVector temp(rhs);
        swap(temp);
        return *this;
    }

private:
    Type* items_;
    std::size_t size_;
    std::size_t capacity_;
};

///////////////////////////////////////////////
// Compartment
///////////////////////////////////////////////
template <typename Type>
bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}

#endif //TMP_SIMPLEVECTOR_H