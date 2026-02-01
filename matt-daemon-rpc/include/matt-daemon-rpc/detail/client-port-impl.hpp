#ifndef MATT_DAEMON_RPC_DETAIL_CLIENT_PORT_IMPL
#define MATT_DAEMON_RPC_DETAIL_CLIENT_PORT_IMPL

#include <cassert>
#include <meta>

#include "matt-daemon-rpc/concepts.hpp"

namespace matt_daemon_rpc::detail {

template <std::meta::info IService>
  requires Service<IService>
class ClientPortImpl {
 public:
  [[nodiscard]] static auto Make() noexcept -> ClientPortImpl {
    assert(false && "Not implemented");
    return ClientPortImpl{};
  }

  template <std::meta::info Func, typename... Args>
    requires Method<IService, Func, Args...>
  [[nodiscard]] auto Call([[maybe_unused]] Args&&... args) noexcept ->
      typename[:std::meta::return_type_of(Func):] {
    return {};
  }

 protected:
  explicit ClientPortImpl<IService>() noexcept = default;
};

}  // namespace matt_daemon_rpc::detail

#endif  // MATT_DAEMON_RPC_DETAIL_CLIENT_PORT_IMPL