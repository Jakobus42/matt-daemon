#include <matt-daemon-interface/bar/bar.hpp>
#include <meta>
#include <print>

#include "matt-daemon-rpc/foo.hpp"

namespace {

template <std::meta::info Func>
constexpr auto policy_has_callable_attribute() -> bool {
  template for (constexpr auto anno :
                std::define_static_array(std::meta::annotations_of(Func))) {
    if constexpr (anno == ^^matt_daemon_rpc::callable) {
      return true;
    }
  }
  return false;
}

template <std::meta::info Func, std::meta::info Namespace>
constexpr auto policy_is_in_namespace() -> bool {
  constexpr auto ctx = std::meta::access_context::current();

  template for (constexpr auto member : std::define_static_array(
                    std::meta::members_of(Namespace, ctx))) {
    if constexpr (member == Func) {
      return true;
    }
    if constexpr (std::meta::is_namespace(member)) {
      if constexpr (is_function_in_namespace<Func, member>()) {
        return true;
      }
    }
    if constexpr (std::meta::is_type(member)) {
      template for (constexpr auto class_member : std::define_static_array(
                        std::meta::members_of(member, ctx))) {
        if constexpr (class_member == Func) {
          return true;
        }
      }
    }
  }
  return false;
}

template <std::meta::info Namespace>
struct RpcClient {
  template <std::meta::info Func, typename... Args>
  auto Call([[maybe_unused]] Args&&... args) -> auto {
    static_assert(policy_is_in_namespace<Func, Namespace>(),
                  "Function must be in this namespace");
    static_assert(policy_has_callable_attribute<Func>(),
                  "Function is not callable");

    constexpr auto func_name = std::meta::identifier_of(Func);
    std::println("calling RPC: {}", func_name);

    std::println("Would send {} argument(s) over network...",
                 sizeof...(Args));
  }
};

}  // namespace

auto main() -> int {
  RpcClient<^^matt_daemon_interface::bar> client;

  using matt_daemon_interface::bar::BarService;
  client.Call<^^BarService::GetSomeStuff>("hey");

  return 0;
}