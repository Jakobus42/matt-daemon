#ifndef MATT_DAEMON_RPC_FOO
#define MATT_DAEMON_RPC_FOO

#include <print>

namespace matt_daemon_rpc {

inline auto foo() -> int {
  std::print("Hello World!\n");
  return 1;
}

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_FOO
