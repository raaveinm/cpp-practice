#include "bimap.h"

#include <string>
#include <unordered_map>

struct BiMap::Impl {
    std::unordered_map<std::string, std::string> key_val;
    std::unordered_map<std::string, std::string> val_key;
};

BiMap::BiMap() : impl_(std::make_unique<Impl>()) {}

BiMap::BiMap(const BiMap& other) {
    if (other.impl_) {
        impl_ = std::make_unique<Impl>(*other.impl_);
    }
}

BiMap::BiMap(BiMap &&) noexcept = default;


BiMap& BiMap::operator=(const BiMap& other) {
    if (this != &other) {
        BiMap temp(other);
        std::swap(impl_, temp.impl_);
    }
    return *this;
}

BiMap& BiMap::operator=(BiMap&&) noexcept = default;

BiMap::~BiMap() = default;

bool BiMap::Add(const std::string_view key, const std::string_view value) {
    if (!impl_) {
        impl_ = std::make_unique<Impl>();
    }
    if (impl_->key_val.contains(std::string(key)) || impl_->val_key.contains(std::string(value)))
        return false;

    std::string key_str(key);
    std::string val_str(value);
    auto [it_kv, success_kv] = impl_->key_val.emplace(key_str, val_str);

    try {
        impl_->val_key.emplace(val_str, key_str);
    } catch (...) {
        impl_->key_val.erase(it_kv);
        throw;
    }
    return true;
}

std::optional<std::string_view> BiMap::FindValue(const std::string_view key) const noexcept {
    if (!impl_) return std::nullopt;
    if (const auto it = impl_->key_val.find(std::string(key)); it != impl_->key_val.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<std::string_view> BiMap::FindKey(const std::string_view value) const noexcept {
    if (!impl_) return std::nullopt;

    if (const auto it = impl_->val_key.find(std::string(value)); it != impl_->val_key.end()) {
        return it->second;
    }
    return std::nullopt;
}