#ifndef MKCORO_INTERFACE_HPP
#define MKCORO_INTERFACE_HPP
#include <concepts>

namespace emcoro
{
    /// @brief concept requiring the required user-defined functions for an awaitable function.
    template <class Awaitable>
    concept await_functor = requires {
                                {
                                    Awaitable::isResumable()
                                    } -> std::same_as<bool>;

                                Awaitable::getResult();
                            };

    /// @brief concept requiring that a promise type has the function used for polling.
    template <class promise_type>
    concept task_promise = requires(promise_type obj) {
                               {
                                   obj.isResumable()
                                   } -> std::same_as<bool>;
                           };

    /// @brief Concept requiring the user-provided exeption handling functions.
    template <class Impl>
    concept ExceptionHandler = requires {
                                   Impl::fatal();
                               };

};

#endif
/// @file interface.hpp
/// @author Mattis Kieffer (mattis.kieffer@hotmail.de)
/// @brief concepts defining the interfaces to templates.
/// @version 1.0
/// @date 2023-01-02
/// @copyright Copyright (c) 2023
