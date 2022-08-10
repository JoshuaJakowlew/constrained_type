#include <iostream>
#include <string>
#include <tuple>
#include <optional>
#include <concepts>
#include <memory>

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

int main()
{
    auto x = constrained_type<std::string,
        [](auto&& x) { return !x.empty(); },
        [](auto&& x) { return x.starts_with("Hello"); }
    >{"!!!Hello world!!!"};

    auto y = constrained_type<std::string,
        [](auto&& x) { return !x.empty(); },
        [](auto&& x) { return x.starts_with("Hello"); }
    >{"Hello world"};

    std::cout << x.value_or("Ooops") << std::endl;
    std::cout << *y << std::endl;
    // x = y;
}