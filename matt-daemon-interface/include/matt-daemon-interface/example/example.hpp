#ifndef MATT_DAEMON_INTERFACE_EXAMPLE_EXAMPLE
#define MATT_DAEMON_INTERFACE_EXAMPLE_EXAMPLE

// TODO(jsadjina): make this better

#include <matt-daemon-rpc/annotations.hpp>
#include <matt-daemon-rpc/result.hpp>
#include <matt-daemon-rpc/strong-type.hpp>

namespace matt_daemon_interface::example {

struct[[= matt_daemon_rpc::service]] PizzaService final {
  using IsOpen = matt_daemon_rpc::StrongType<bool, struct IsOpenTag>;

  [[= matt_daemon_rpc::method]] auto IsStoreOpen()
      -> matt_daemon_rpc::Result<IsOpen>;
};

}  // namespace matt_daemon_interface::example

#endif  // MATT_DAEMON_INTERFACE_EXAMPLE_EXAMPLE
