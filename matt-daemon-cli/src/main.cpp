#include <matt-daemon-interface/bar/bar.hpp>
#include <matt-daemon-interface/foo/foo.hpp>
#include <matt-daemon-rpc/foo.hpp>

auto main() -> int {
  matt_daemon_rpc::foo();
  matt_daemon_interface::bar::bar();
  matt_daemon_interface::foo::foo();
}
