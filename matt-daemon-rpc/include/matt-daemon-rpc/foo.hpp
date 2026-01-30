#ifndef MATT_DAEMON_RPC_FOO
#define MATT_DAEMON_RPC_FOO

#include <cstdint>

namespace matt_daemon_rpc {

struct service_t {};
constexpr service_t service;

struct callable_t {};
constexpr callable_t callable;

struct async_t {};
constexpr async_t async;

struct subscribable_t {};
constexpr subscribable_t subscribeable;

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_FOO
