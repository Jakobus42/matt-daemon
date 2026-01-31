#ifndef MATT_DAEMON_RPC_CONCEPTS
#define MATT_DAEMON_RPC_CONCEPTS

#include <matt-daemon-rpc/annotations.hpp>
#include <matt-daemon-rpc/detail/concepts.hpp>
#include <meta>

// TODO(jsadjina): get name of the service for error reporting
// write more tests for those
// check if callable return matt_daemon_rpc::Result

namespace matt_daemon_rpc {

template <std::meta::info Class>
concept Service =
    detail::HasAnnotation<Class, decltype(::matt_daemon_rpc::service)>() &&
    std::meta::is_class_type(Class);

template <std::meta::info IService, std::meta::info Func, typename... Args>
concept Callable = Service<IService> && detail::IsChildOf<IService, Func>() &&
                   detail::DoParameterTypesMatch<Func, Args...>();

template <std::meta::info IService, std::meta::info Func, typename... Args>
concept Method =
    Callable<IService, Func, Args...> &&
    detail::HasAnnotation<Func, decltype(::matt_daemon_rpc::method)>();

}  // namespace matt_daemon_rpc

#endif  // MATT_DAEMON_RPC_CONCEPTS