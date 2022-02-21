#ifndef BIT_UTILS_GUARD__
#define BIT_UTILS_GUARD__

#include <type_traits>

template <typename Enum>
struct EnableBitMaskOperators
{
    static const bool enable = false;
};

template <typename Enum>
inline constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator|(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <typename Enum>
inline typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type& operator|=(
    Enum& lhs,
    Enum rhs)
{
    return lhs = lhs | rhs;
}

template <typename Enum>
inline constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator&(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <typename Enum>
inline typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type& operator&=(
    Enum& lhs,
    Enum rhs)
{
    return lhs = lhs & rhs;
}

template <typename Enum>
inline constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator~(Enum arg)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(~static_cast<underlying>(arg));
}

#define ENABLE_BITMASK_OPERATORS(x)                                                      \
    template <>                                                                          \
    struct EnableBitMaskOperators<x>                                                     \
    {                                                                                    \
        static const bool enable = true;                                                 \
    }

#endif /* BIT_UTILS_GUARD__ */
