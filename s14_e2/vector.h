#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <new>
#include <utility>


///////////////////////////////////////////////
// Raw Memory
///////////////////////////////////////////////
template <typename T>
class RawMemory {
public:
    RawMemory() = default;
    explicit RawMemory(const size_t capacity)
        : buffer_(Allocate(capacity))
        , capacity_(capacity) {
    }

    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory&) = delete;
    RawMemory(RawMemory&& other) noexcept
        : buffer_(other.buffer_)
        , capacity_(other.capacity_) {
        other.buffer_ = nullptr;
        other.capacity_ = 0;
    }
    RawMemory& operator=(RawMemory&& other) noexcept {
        if (this != &other) {
            Deallocate(buffer_);
            buffer_ = other.buffer_;
            capacity_ = other.capacity_;
            other.buffer_ = nullptr;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~RawMemory() {
        Deallocate(buffer_);
    }

    T* operator+(size_t offset) noexcept {
        assert(offset <= capacity_);
        return buffer_ + offset;
    }
    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }
    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }
    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }
    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }
    const T* GetAddress() const noexcept {
        return buffer_;
    }
    T* GetAddress() noexcept {
        return buffer_;
    }
    [[nodiscard]] size_t Capacity() const {
        return capacity_;
    }
private:
    static T* Allocate(const size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }
    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};


///////////////////////////////////////////////
// Vector
///////////////////////////////////////////////

template <typename T>
class Vector {
public:
    using iterator = T*;
    using const_iterator = const T*;

    // Constructor

    Vector() = default;

    explicit Vector(const size_t size) : data_(size) {
        std::uninitialized_value_construct_n(data_.GetAddress(), size);
        size_ = size;
    }

    Vector(const Vector& other) : data_(other.size_) {
        std::uninitialized_copy_n(other.data_.GetAddress(), other.size_, data_.GetAddress());
        size_ = other.size_;
    }

    Vector(Vector&& other) noexcept
        : data_(std::move(other.data_))
        , size_(std::exchange(other.size_, 0)) {
    }

    Vector& operator=(Vector&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~Vector() {
        std::destroy_n(data_.GetAddress(), size_);
    }

    // Iterator

    iterator begin() noexcept { return data_.GetAddress(); }
    iterator end() noexcept { return data_.GetAddress() + size_; }
    const_iterator begin() const noexcept { return data_.GetAddress(); }
    const_iterator end() const noexcept { return data_.GetAddress() + size_; }
    const_iterator cbegin() const noexcept { return data_.GetAddress(); }
    const_iterator cend() const noexcept { return data_.GetAddress() + size_; }

    // Member functions

    void Reserve(const size_t new_capacity) {
        if (new_capacity > data_.Capacity()) {
            RawMemory<T> new_data(new_capacity);
            try {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
                } else {
                    std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
                }
            } catch (...) {
                throw;
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        }
    }

    [[nodiscard]] size_t Size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_t Capacity() const noexcept {
        return data_.Capacity();
    }

    const T& operator[](size_t index) const noexcept {
        assert(index < size_);
        return data_[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            if (other.size_ > data_.Capacity()) {
                Vector new_vector(other);
                Swap(new_vector);
            } else if (other.size_ <= size_) {
                std::copy(other.data_.GetAddress(), other.data_.GetAddress() + other.size_, data_.GetAddress());
                std::destroy_n(data_.GetAddress() + other.size_, size_ - other.size_);
                size_ = other.size_;
            } else {
                std::copy(other.data_.GetAddress(), other.data_.GetAddress() + size_, data_.GetAddress());
                std::uninitialized_copy_n(other.data_.GetAddress() + size_,
                                          other.size_ - size_,
                                          data_.GetAddress() + size_);
                size_ = other.size_;
            }
        }
        return *this;
    }

    void Swap(Vector& other) noexcept {
        data_.Swap(other.data_);
        std::swap(size_, other.size_);
    }

    void PushBack(const T& value) {
        if (size_ == data_.Capacity()) {
            size_t new_capacity = data_.Capacity() == 0 ? 1 : data_.Capacity() * 2;
            RawMemory<T> new_data(new_capacity);
            T* new_elem_ptr = new_data.GetAddress() + size_;
            std::construct_at(new_elem_ptr, value);
            try {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
                } else {
                    std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
                }
            } catch (...) {
                std::destroy_at(new_elem_ptr);
                throw;
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        } else {
            std::construct_at(data_.GetAddress() + size_, value);
        }
        ++size_;
    }

    void PushBack(T&& value) {
        if (size_ == data_.Capacity()) {
            size_t new_capacity = data_.Capacity() == 0 ? 1 : data_.Capacity() * 2;
            RawMemory<T> new_data(new_capacity);
            T* new_elem_ptr = new_data.GetAddress() + size_;
            std::construct_at(new_elem_ptr, std::move(value));
            try {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
                } else {
                    std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
                }
            } catch (...) {
                std::destroy_at(new_elem_ptr);
                throw;
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        } else {
            std::construct_at(data_.GetAddress() + size_, std::move(value));
        }
        ++size_;
    }

    void PopBack() noexcept {
        --size_;
        std::destroy_at(data_.GetAddress() + size_);
    }

    Vector& Resize(const size_t new_size) {
        if (new_size > size_) {
            Reserve(new_size);
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
            size_ = new_size;
        } else if (new_size < size_) {
            std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
            size_ = new_size;
        }
        return *this;
    }

    template <typename... Args>
    T& EmplaceBack(Args&&... args) {
        if (size_ == data_.Capacity()) {
            size_t new_capacity = data_.Capacity() == 0 ? 1 : data_.Capacity() * 2;
            RawMemory<T> new_data(new_capacity);
            T* new_elem_ptr = new_data.GetAddress() + size_;
            std::construct_at(new_elem_ptr, std::forward<Args>(args)...);
            try {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
                } else {
                    std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
                }
            } catch (...) {
                std::destroy_at(new_elem_ptr);
                throw;
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
            ++size_;
        } else {
            std::construct_at(data_.GetAddress() + size_, std::forward<Args>(args)...);
            ++size_;
        }
        return data_[size_ - 1];
    }

template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        assert(pos >= cbegin() && pos <= cend());
        size_t index = pos - cbegin();

        if (size_ == data_.Capacity()) { // re alloc
            size_t new_capacity = data_.Capacity() == 0 ? 1 : data_.Capacity() * 2;
            RawMemory<T> new_data(new_capacity);
            T* new_elem_ptr = new_data.GetAddress() + index;
            std::construct_at(new_elem_ptr, std::forward<Args>(args)...);

            try {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
                    std::uninitialized_move_n(data_.GetAddress(), index, new_data.GetAddress());
                else
                    std::uninitialized_copy_n(data_.GetAddress(), index, new_data.GetAddress());

                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
                    std::uninitialized_move_n(data_.GetAddress() + index, size_ - index, new_data.GetAddress() + index + 1);
                else
                    std::uninitialized_copy_n(data_.GetAddress() + index, size_ - index, new_data.GetAddress() + index + 1);

            } catch (...) {
                std::destroy_at(new_elem_ptr);
                throw;
            }

            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);

        } else { // re alloc not needed

            if (index != size_) {
                T temp(std::forward<Args>(args)...);
                std::construct_at(data_.GetAddress() + size_, std::move(data_[size_ - 1]));
                std::move_backward(data_.GetAddress() + index, data_.GetAddress() + size_ - 1, data_.GetAddress() + size_);
                data_[index] = std::move(temp);
            } else {
                std::construct_at(data_.GetAddress() + size_, std::forward<Args>(args)...);
            }
        }

        ++size_;
        return begin() + index;
    }

    iterator Erase(const_iterator pos) {
        assert(pos >= cbegin() && pos < cend());
        size_t index = pos - cbegin();
        std::move(data_.GetAddress() + index + 1,
                  data_.GetAddress() + size_,
                  data_.GetAddress() + index);
        --size_;
        std::destroy_at(data_.GetAddress() + size_);
        return begin() + index;
    }

    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }

    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::move(value));
    }


private:
    RawMemory<T> data_;
    size_t size_ = 0;
};