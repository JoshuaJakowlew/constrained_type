#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <memory>

#include <constrained_optional.hpp>

using namespace st;

// Non-empty vector constraint

// https://t.me/supapro/1138005
// Read all the thread
constexpr auto non_empty_constraint()
{
    return [](auto&& vec) {
        return !vec.empty();
    };
}

template <typename T>
using non_empty = constrained_optional<std::vector<T>,
    non_empty_constraint()
>;

// Safe head function
template <typename T>
std::optional<T> head(non_empty<T> && vec)
{
    if (vec)
        return (*vec)[0];
    return std::nullopt;
}

// String, starting with "Hello" constraint

using hello_str = constrained_optional<std::string,
    [](auto&& x) { return x.starts_with("Hello"); }
>;

// Event int which can't be null constraint

using even_not_null_int = constrained_optional<int,
    [](auto&& x) { return x     != 0; },
    [](auto&& x) { return x % 2 == 0; }
>;

int main()
{
    auto v1 = non_empty<int>{};
    auto v2 = non_empty<std::string>{"A", "B", "C", "D"};

    std::cout << "\n[non_empty<T>]\n";
    std::cout << head(std::move(v1)).value_or(-1) << std::endl;
    std::cout << head(std::move(v2)).value_or("Ooops") << std::endl;

    auto s1 = hello_str{"!!!Hello world!!!"};
    auto s2 = hello_str{"Hello world"};

    std::cout << "\n[hello_str]\n";
    std::cout << s1.value_or("Ooops") << std::endl;
    std::cout << s2.value_or("Ooops") << std::endl;

    // Compile time constraints
    constexpr auto i1 = even_not_null_int{0};
    constexpr auto i2 = even_not_null_int{1};
    constexpr auto i3 = even_not_null_int{2};
    
    std::cout << "\n[even_not_null_int]\n";
    std::cout << i1.value_or(-1) << std::endl;
    std::cout << i2.value_or(-1) << std::endl;
    std::cout << i3.value_or(-1) << std::endl;
}
