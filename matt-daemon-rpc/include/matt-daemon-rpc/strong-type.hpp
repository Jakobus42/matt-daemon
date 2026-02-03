#ifndef MATT_DAEMON_RPC_STRONG_TYPE
#define MATT_DAEMON_RPC_STRONG_TYPE

// TODO(jsadjina): think about if this should be part of the api

#include <meta>

namespace matt_daemon_rpc {

/**
 * @brief TBD
 *
 * @tparam T
 * @tparam Tag
 */
template <typename T, typename Tag>
class StrongType {
 public:
  using ValueType = T;
  using TagType = Tag;

  explicit constexpr StrongType(T const& value) noexcept(
      std::is_nothrow_copy_constructible_v<T>)
      : value_(value) {
  }

  explicit constexpr StrongType(T&& value) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : value_(std::move(value)) {
  }

  constexpr auto get() noexcept -> T& {
    return value_;
  }

  [[nodiscard]] constexpr auto get() const noexcept
      -> std::remove_reference_t<T> const& {
    return value_;
  }

 private:
  T value_;
};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_STRONG_TYPE