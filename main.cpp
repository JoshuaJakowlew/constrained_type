#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <memory>

#include <constrained_type.hpp>

using namespace st;

// https://t.me/supapro/1138005
// Read all the thread
constexpr auto non_empty_constraint()
{
    return [](auto&& vec) {
        return !vec.empty();
    };
}

template <typename T>
using non_empty = constrained_type<std::vector<T>,
    non_empty_constraint()
>;

using hello_str = constrained_type<std::string,
    [](auto&& x) { return !x.empty(); },
    [](auto&& x) { return x.starts_with("Hello"); }
>;

using even_not_null_int = constrained_type<int,
    [](auto&& x) { return x     != 0; },
    [](auto&& x) { return x % 2 == 0; }
>;

template <typename T>
T foo(non_empty<T> && vec)
{
    return vec.value()[0];
}

int main()
{
    auto x = hello_str{"!!!Hello world!!!"};
    auto y = hello_str{"Hello world"};

    std::cout << x.value_or("Ooops") << std::endl;
    std::cout << y.value() << std::endl;

    auto z = non_empty<int>{1, 2, 3, 4};
    std::cout << foo(std::move(z)) << std::endl;

    // Compile time constraints
    constexpr auto i1 = even_not_null_int{0};
    constexpr auto i2 = even_not_null_int{1};
    constexpr auto i3 = even_not_null_int{2};
    std::cout << i1.value_or(-1) << std::endl;
    std::cout << i2.value_or(-1) << std::endl;
    std::cout << i3.value_or(-1) << std::endl;
}
