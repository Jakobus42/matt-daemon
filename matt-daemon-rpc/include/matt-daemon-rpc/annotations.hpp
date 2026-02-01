#ifndef MATT_DAEMON_RPC_ANNOTATIONS
#define MATT_DAEMON_RPC_ANNOTATIONS

namespace matt_daemon_rpc {

// annotations definetly have more potential xd
// ideas to add would be version, deprecated,
// description (maybe interesting for cli)

/**
 * @brief Annotation to mark a struct/class as an RPC service.
 *
 * Annotatd types must satisfy the @ref matt_daemon_rpc::Service concept.
 *
 * Usage:
 * @code
 * struct [[= service]] MyService { ... };
 * @endcode
 */
inline constexpr struct {
} service{};

/**
 * @brief Annotation to mark a member function as an RPC method.
 *
 * Annotatd methods must satisfy the @ref matt_daemon_rpc::Method concept.
 *
 * Usage:
 * @code
 * [[= method]] auto MyMethod() -> AsyncResult<bool>;
 * @endcode
 */
inline constexpr struct {
} method{};

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_ANNOTATIONS