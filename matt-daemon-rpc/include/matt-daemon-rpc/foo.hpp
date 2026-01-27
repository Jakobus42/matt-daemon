#ifndef MATT_DAEMON_RPC_FOO
#define MATT_DAEMON_RPC_FOO

#include <print>

namespace matt_daemon_rpc {

inline auto foo() -> void {
  std::print("Hello from matt-daemon-rpc/foo.hpp!\n");
}

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_FOO
