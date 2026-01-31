#ifndef MATT_DAEMON_RPC_RESULT
#define MATT_DAEMON_RPC_RESULT

#include <matt-daemon-rpc/error.hpp>

// TODO(jsadjina) write this

namespace matt_daemon_rpc {

template <typename T, typename Err = Error<>>
struct Result final {};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_RESULT