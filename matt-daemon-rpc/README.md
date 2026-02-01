# matt-daemon-rpc

A modern C++26 RPC library using annotations, reflections and concepts.

## Components

- `client-port.hpp` - Client to invoke RPC service Callables
- `concepts.hpp` - Concepts enforce compile time validation of RPC interfaces
- `annotations.hpp` - Annotations for concepts (e.g. service, method, ...)
- `async-result.hpp` - Return type of a valid Callable
- `error.hpp` - Error type of a valid Callable
- `strong-type.hpp` - Type safe wrapper for primitives
