#ifndef MATT_DAEMON_INTERFACE_FOO_FOO
#define MATT_DAEMON_INTERFACE_FOO_FOO

#include <print>

namespace matt_daemon_interface::foo {

inline auto foo() -> void {
  std::print("Hello from matt-daemon-interface/foo.hpp\n");
}

}  // namespace matt_daemon_interface::foo

#endif  // MATT_DAEMON_INTERFACE_FOO_FOO
