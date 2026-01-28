// from:
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p2996r13.html#enum-to-string

#include <cstdint>
#include <meta>
#include <print>

namespace {

template <typename E, bool Enumerable = std::meta::is_enumerable_type(^^E)>
  requires std::is_enum_v<E>
constexpr auto enum_to_string(E value) -> std::string_view {
  if constexpr (Enumerable) {
    // NOLINTBEGIN - false positive
    template for (constexpr auto elem :
                  std::define_static_array(std::meta::enumerators_of(^^E))) {
    // NOLINTEND
      if (value == [:elem:]) {
        return std::meta::identifier_of(elem);
      }
    }
  }
  return "<unnamed>";
}

}  // namespace

auto main() -> int {
  enum Color : std::uint8_t;
  static_assert(enum_to_string(Color(0)) == "<unnamed>");
  std::println("Color 0: {}", enum_to_string(Color(0)));  // prints '<unnamed>'

  enum Color : std::uint8_t { red, green, blue };
  static_assert(enum_to_string(Color::red) == "red");
  static_assert(enum_to_string(Color(3)) == "<unnamed>");
  std::println("Color 0: {}", enum_to_string(Color(0)));  // prints 'red'
}