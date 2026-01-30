#ifndef MATT_DAEMON_INTERFACE_BAR_BAR
#define MATT_DAEMON_INTERFACE_BAR_BAR

#include <matt-daemon-rpc/foo.hpp>

namespace matt_daemon_interface::bar {

struct[[= matt_daemon_rpc::service]] BarService {
  [[= matt_daemon_rpc::callable]] auto GetSomeStuff(int aaa) -> void;
};

}  // namespace matt_daemon_interface::bar

#endif  // MATT_DAEMON_INTERFACE_BAR_BAR
