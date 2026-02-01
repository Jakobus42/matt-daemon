#ifndef MATT_DAEMON_RPC_CONCEPTS
#define MATT_DAEMON_RPC_CONCEPTS

#include <meta>

#include "matt-daemon-rpc/annotations.hpp"
#include "matt-daemon-rpc/async-result.hpp"
#include "matt-daemon-rpc/detail/concepts.hpp"

// TODO(jsadjina): get name of the service for error reporting

namespace matt_daemon_rpc {

template <std::meta::info Class>
concept Service = detail::HasAnnotation<Class, decltype(service)>() &&
                  std::meta::is_class_type(Class);

template <std::meta::info IService, std::meta::info Func, typename... Args>
concept Callable = Service<IService> && detail::IsChildOf<IService, Func>() &&
                   detail::DoParameterTypesMatch<Func, Args...>() &&
                   detail::ReturnsTemplate<Func, ^^AsyncResult>();

template <std::meta::info IService, std::meta::info Func, typename... Args>
concept Method = Callable<IService, Func, Args...> &&
                 detail::HasAnnotation<Func, decltype(method)>();

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_CONCEPTS