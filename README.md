# constrained_type

constrained_type provides utility types that simplify invariant checks on types.

## What's in there

- constrained_type<T, Constraints...> - type containig a set of predicates in its type which must be evaluated to true at construction time, or exception will be raised.

- basic_constrained_optional<T, Optional, Constraints...> - same as `constrained_type`, but uses optional, instead of throwing exceptions. This type is polymorphic over optionals, so you aren't bound to `std::optional`.

- constrained_optional<T, Constraints...> - alias on `basic_constrained_optional`, parametrized over `std::optional<T>`

## Overhead

Both `constrained_type` and `basic_constrained_optional` performs check on predicates on construction. Actually, check is not performed in copy and move constructors - there is no need for it.

So, there is overhead at construction time, but if invariants must be checked anyway - it's zero-cost to use these types. Look at examples to see the difference.

`basic_constrained_optional` is polymophic over optionals. In general case there is space overhead for storing the state of the optional type inside it. But it's possible to use any implementation of something like `compact_optional<T, Null>` type, that uses one of T's values to represent empty state. In this case there is no overhead in size.
