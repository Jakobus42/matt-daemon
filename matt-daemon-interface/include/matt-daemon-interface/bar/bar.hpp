#ifndef MATT_DAEMON_INTERFACE_BAR_BAR
#define MATT_DAEMON_INTERFACE_BAR_BAR

#include <print>

namespace matt_daemon_interface::bar {

inline auto bar() -> void {
  std::print("Hello from matt-daemon-interface/bar.hpp\n");
}

}  // namespace matt_daemon_interface::bar

#endif  // MATT_DAEMON_INTERFACE_BAR_BAR
