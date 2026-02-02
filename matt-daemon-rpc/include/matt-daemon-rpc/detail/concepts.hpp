#ifndef MATT_DAEMON_RPC_DETAIL_CONCEPTS
#define MATT_DAEMON_RPC_DETAIL_CONCEPTS

#include <algorithm>
#include <array>
#include <meta>
#include <ranges>
#include <vector>

namespace matt_daemon_rpc::detail {

template <std::meta::info Func, typename... Args>
  requires(std::meta::is_function(Func))
[[nodiscard]] consteval auto DoParameterTypesMatch() noexcept -> bool {
  auto params = std::meta::parameters_of(Func);
  std::array<std::meta::info, sizeof...(Args)> args{^^Args...};
  if (params.size() != sizeof...(Args)) {
    return false;
  }

  return std::ranges::all_of(
      std::views::zip(std::move(params), std::move(args)),
      [](auto const& pair) consteval -> bool {
        auto const& [param, arg_type] = pair;
        return std::meta::type_of(param) == arg_type;
      });
}

template <std::meta::info Parent, std::meta::info T>
[[nodiscard]] consteval auto IsChildOf() noexcept -> bool {
  // NOLINTNEXTLINE - this turned out to be a pain with ranges
  for (auto parent = std::meta::parent_of(T); parent != ^^::;
       parent = std::meta::parent_of(parent)) {
    if (parent == Parent) {
      return true;
    }
  }
  return false;
}

template <std::meta::info T, typename AnnotationType>
[[nodiscard]] consteval auto HasAnnotation() noexcept -> bool {
  return !std::meta::annotations_of(T, ^^AnnotationType).empty();
}

template <std::meta::info Func, std::meta::info Template>
[[nodiscard]] consteval auto ReturnsTemplate() noexcept -> bool {
  auto const ret_type = std::meta::return_type_of(Func);
  if (!std::meta::has_template_arguments(ret_type)) {
    return false;
  }
  return std::meta::template_of(ret_type) == Template;
}

}  // namespace matt_daemon_rpc::detail

#endif  // MATT_DAEMON_RPC_DETAIL_CONCEPTS