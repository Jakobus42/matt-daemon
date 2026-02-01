#ifndef MATT_DAEMON_RPC_CLIENT_PORT
#define MATT_DAEMON_RPC_CLIENT_PORT

#include <meta>

#include "matt-daemon-rpc/concepts.hpp"
#include "matt-daemon-rpc/detail/client-port-impl.hpp"

// TODO(jsadjina): consider not including impl here so only mock impl is
// included in tests (or smth like that)

namespace matt_daemon_rpc {

template <std::meta::info IService,
          typename Impl = detail::ClientPortImpl<IService>>
  requires Service<IService>
class ClientPort final {
 public:
  [[nodiscard]] static auto Make() noexcept -> ClientPort {
    return ClientPort{Impl::Make()};
  }

  template <std::meta::info Func, typename... Args>
    requires Method<IService, Func, Args...>
  [[nodiscard]] auto Call(Args&&... args) noexcept ->
      typename[:std::meta::return_type_of(Func):] {
    return impl_.template Call<Func>(std::forward<Args>(args)...);
  }

 private:
  explicit ClientPort(Impl impl) : impl_(std::move(impl)) {
  }

  detail::ClientPortImpl<IService> impl_;
};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_CLIENT_PORT