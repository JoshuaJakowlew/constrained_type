#include <concepts>
#include <stdexcept>

namespace st
{
    template <typename T, std::predicate<T> auto... Constraints>
    class constrained_type
    {
    public:
        constexpr constrained_type(auto&&... args)
            : _value{std::forward<decltype(args)>(args)...}
        {
            bool satisfied = (Constraints(_value) && ...);
            if (!satisfied)
            {
                throw std::logic_error{"Constraints not satisfied"};
            }
        }

        constexpr constrained_type(constrained_type const & other) = default;

        constexpr constrained_type(constrained_type&& other) noexcept
        {
            swap(_value, other._value);
        }

        constexpr constrained_type & operator=(constrained_type const & other) = default;

        constexpr constrained_type & operator=(constrained_type&& other) noexcept
        {
            swap(_value, other._value);
            return *this;
        }

        [[nodiscard]] constexpr auto value() const & -> T const &
        {
            return _value;
        }

        [[nodiscard]] constexpr auto value() const && -> T const &&
        {
            return _value;
        }

        [[nodiscard]] constexpr auto operator*() const & noexcept -> T const &
        {
            return value();
        }

        [[nodiscard]] constexpr auto operator*() const && noexcept -> T const &&
        {
            return value();
        }

        [[nodiscard]] constexpr operator T() const noexcept
        {
            return _value;
        }
    private:
        T _value;
    };
} // namespace st