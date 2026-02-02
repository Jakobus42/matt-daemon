#ifndef MATT_DAEMON_RPC_RESULT
#define MATT_DAEMON_RPC_RESULT

#include <expected>
#include <future>
#include <tuple>

#include "matt-daemon-rpc/error.hpp"

namespace matt_daemon_rpc {

/**
 * @brief TBD
 *
 * @tparam T
 * @tparam Err
 */
template <typename T, typename Err = std::monostate>
class FutureResult final {
 public:
  using ValueType = T;
  using ErrorType = Error<Err>;
  using ResultType = std::expected<ValueType, ErrorType>;
  using FutureType = std::future<ResultType>;

  [[nodiscard]] static auto Make(ValueType value) noexcept -> FutureResult {
    std::ignore = value;
    return FutureResult{};
  }
};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_RESULT