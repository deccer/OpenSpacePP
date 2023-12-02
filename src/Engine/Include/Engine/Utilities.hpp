#pragma once

#include <functional>
#include <cstdint>

template <typename T>
concept IsContainer = requires (T value)
{
    typename T::value_type;
    { value.size() } -> std::convertible_to<uint64_t>;
};

template <typename TContainer>
    requires IsContainer<TContainer>
constexpr uint64_t SizeInBytes(const TContainer& container) noexcept
{
    return container.size() * sizeof(typename TContainer::value_type);
}

template <typename T>
constexpr uint64_t SizeInBytes(const T&) noexcept
{
    return sizeof(T);
}