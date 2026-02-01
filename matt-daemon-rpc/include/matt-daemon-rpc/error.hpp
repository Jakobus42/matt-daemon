#ifndef MATT_DAEMON_RPC_ERROR
#define MATT_DAEMON_RPC_ERROR

// TODO(jsadjina) write this

#include <variant>

namespace matt_daemon_rpc {

/**
 * @brief TBD
 *
 * @tparam Err
 */
template <typename Err = std::monostate>
struct Error final {};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_ERROR