#ifndef MATT_DAEMON_RPC_RESULT
#define MATT_DAEMON_RPC_RESULT

#include "matt-daemon-rpc/error.hpp"

namespace matt_daemon_rpc {

/**
 * @brief TBD
 *
 * @tparam T
 * @tparam Err
 */
template <typename T, typename Err = std::monostate>
struct AsyncResult final {
  using ErrorType = Error<Err>;
  using ValueType = T;
};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_RESULT