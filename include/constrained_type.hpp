#include <concepts>
#include <optional>

namespace st
{
    template <typename Optional, typename T>
    concept optional = std::constructible_from<T> && requires (Optional opt)
    {
        opt.value();
        static_cast<bool>(opt);
    };

    template <typename T, optional<T> Optional, std::predicate<T> auto... Constraints>
    class basic_constrained_type
    {
    public:
        constexpr basic_constrained_type(auto&&... args)
        {
            auto value = T{std::forward<decltype(args)>(args)...};
            bool satisfied = (Constraints(std::move(value)) && ...);
            if (satisfied)
            {
                _value = std::move(value);
            }
        }

        constexpr basic_constrained_type(basic_constrained_type const & other) = default;

        constexpr basic_constrained_type(basic_constrained_type&& other) noexcept
        {
            swap(_value, other._value);
        }

        constexpr basic_constrained_type & operator=(basic_constrained_type const & other) = default;

        constexpr basic_constrained_type & operator=(basic_constrained_type&& other) noexcept
        {
            swap(_value, other._value);
            return *this;
        }

        [[nodiscard]] constexpr auto value() const & -> T const &
        {
            return _value.value();
        }

        [[nodiscard]] constexpr auto value() const && -> T const &&
        {
            return _value.value();
        }

        [[nodiscard]] constexpr auto operator*() const & noexcept -> T const &
        {
            return value();
        }

        [[nodiscard]] constexpr auto operator*() const && noexcept -> T const &&
        {
            return value();
        }

        template<class U>
        [[nodiscard]] constexpr T value_or( U&& default_value ) const &
        {
            return static_cast<bool>(_value) ? _value.value() : default_value;
        }

        template<class U>
        [[nodiscard]] constexpr T value_or( U&& default_value ) &&
        {
            return static_cast<bool>(_value) ? _value.value() : default_value;
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return static_cast<bool>(_value);
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool
        {
            return static_cast<bool>(_value);
        }
    private:
        Optional _value;
    };

    template <typename T, std::predicate<T> auto... Constraints>
    using constrained_type = basic_constrained_type<T, std::optional<T>, Constraints...>;
} // namespace st