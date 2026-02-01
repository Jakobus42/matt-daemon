# matt-daemon-rpc (**WIP**)

A modern C++26 RPC library using annotations, reflections and concepts.

## Components

- `client-port.hpp` - Client to invoke RPC service Callables
- `concepts.hpp` - Concepts enforce compile time validation of RPC interfaces
- `annotations.hpp` - Annotations for concepts (e.g. service, method, ...)
- `async-result.hpp` - Return type of a valid Callable
- `error.hpp` - Error type of a valid Callable
- `strong-type.hpp` - Type safe wrapper for primitives

## Example

### Interface definition

```c++
#ifndef MATT_DAEMON_INTERFACE_EXAMPLE_EXAMPLE
#define MATT_DAEMON_INTERFACE_EXAMPLE_EXAMPLE

#include <matt-daemon-rpc/annotations.hpp>
#include <matt-daemon-rpc/async-result.hpp>
#include <matt-daemon-rpc/strong-type.hpp>

namespace matt_daemon_interface::example {

struct[[= matt_daemon_rpc::service]] PizzaService final {
  using IsOpen = matt_daemon_rpc::StrongType<bool, struct IsOpenTag>;

  [[= matt_daemon_rpc::method]] auto IsStoreOpen()
      -> matt_daemon_rpc::AsyncResult<IsOpen>;
};

}  // namespace matt_daemon_interface::example

#endif  // MATT_DAEMON_INTERFACE_EXAMPLE_EXAMPLE
```

### Client

```c++
#include <matt-daemon-interface/example/example.hpp>
#include <matt-daemon-rpc/client-port.hpp>

auto main() -> int {
  using matt_daemon_rpc::ClientPort;
  auto client_port =
      ClientPort<^^matt_daemon_interface::example::PizzaService>::Make();

  using matt_daemon_interface::example::PizzaService;
  std::ignore = client_port.Call<^^PizzaService::IsStoreOpen>();
}
```

### Server

**TBD**
