#pragma once

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

template <typename T>
constexpr typename std::underlying_type<T>::type to_underlying(T enumValue) noexcept {
    return static_cast<typename std::underlying_type<T>::type>(enumValue);
}