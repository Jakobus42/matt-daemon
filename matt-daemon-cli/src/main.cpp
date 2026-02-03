#include <matt-daemon-interface/example/example.hpp>
#include <matt-daemon-rpc/client-port.hpp>

auto main() -> int {
  using matt_daemon_rpc::ClientPort;
  auto client_port =
      ClientPort<^^matt_daemon_interface::example::PizzaService>::Make();

  using matt_daemon_interface::example::PizzaService;
  std::ignore = client_port.Call<^^PizzaService::IsStoreOpen>();
}