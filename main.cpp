#include <iostream>
#include <string>
#include <tuple>
#include <optional>
#include <concepts>


template <typename T, std::predicate<T> auto... Constraints>
class constrained_type
{
public:
    constexpr constrained_type(auto&&... args)
    {
        auto value = T{std::forward<decltype(args)>(args)...};
        bool satisfied = (Constraints(std::move(value)) & ...);
        if (satisfied)
        {
            _value = std::move(value);
        }
    }

    [[nodiscard]] constexpr auto value() const & -> T const &
    {
        return _value.value();
    }

    [[nodiscard]] constexpr auto value() const && -> T const &&
    {
        return _value.value();
    }

    template<class U>
    [[nodiscard]] constexpr T value_or( U&& default_value ) const &
    {
        return _value.value_or(std::forward<U>(default_value));
    }

    template<class U>
    [[nodiscard]] constexpr T value_or( U&& default_value ) &&
    {
        return _value.value_or(std::forward<U>(default_value));
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
    std::optional<T> _value;
};

int main()
{
    // constexpr auto x = constrained_type<int,
    //     [](auto x) { return x % 2 == 0; },
    //     [](auto x) { return x != 0; }
    // >{1488};

    auto y = constrained_type<std::string,
        [](auto&& x) { return !x.empty(); },
        [](auto&& x) { return x.starts_with("Hello"); }
    >{"Hello world"};

    // std::cout << x.value_or(-1) << std::endl;
    std::cout << y.value_or("Nope") << std::endl;
}