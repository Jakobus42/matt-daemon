#ifndef MATT_DAEMON_RPC_CONCEPTS
#define MATT_DAEMON_RPC_CONCEPTS

#include <meta>

#include "matt-daemon-rpc/annotations.hpp"
#include "matt-daemon-rpc/async-result.hpp"
#include "matt-daemon-rpc/detail/concepts.hpp"

// TODO(jsadjina): get name of the service for error reporting

namespace matt_daemon_rpc {

/**
 * @brief Validates that a type is a valid RPC service.
 *
 * A service must be a class or struct annotated with @ref
 * matt_daemon_rpc::service.
 *
 * @tparam Class The type to validate as a service
 */
template <std::meta::info Class>
concept Service = detail::HasAnnotation<Class, decltype(service)>() &&
                  std::meta::is_class_type(Class);

/**
 * @brief Validates that a function can be invoked as an RPC call.
 *
 * A callable must belong to a service which satisfies the @ref
 * matt_daemon_rpc::Service concept, have matching parameter types, and return
 * @ref matt_daemon_rpc::AsyncResult.
 *
 * @tparam IService The service containing the method
 * @tparam Func The method function
 * @tparam Args The parameter types the method acepts
 *
 */
template <std::meta::info IService, std::meta::info Func, typename... Args>
concept Callable = Service<IService> && detail::IsChildOf<IService, Func>() &&
                   detail::DoParameterTypesMatch<Func, Args...>() &&
                   detail::ReturnsTemplate<Func, ^^AsyncResult>();

/**
 * @brief Validates that a function is a RPC method.
 *
 * A method must satisfy the @ref matt_daemon_rpc::Callable concept and be
 * annotated with @ref matt_daemon_rpc::method.
 *
 * @tparam IService The service containing the method
 * @tparam Func The method function
 * @tparam Args The parameter types the method acepts
 *
 */
template <std::meta::info IService, std::meta::info Func, typename... Args>
concept Method = Callable<IService, Func, Args...> &&
                 detail::HasAnnotation<Func, decltype(method)>();

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_CONCEPTS