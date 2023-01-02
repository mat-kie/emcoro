#ifndef MKCORO_AWAITABLE_HPP
#define MKCORO_AWAITABLE_HPP
#include <coroutine>
#include "interface.hpp"

namespace emcoro
{
    /// @brief templated struct providing the nececarry methods for the co_await keyword.
    /// @tparam F structure containing the user-defined functions @see await_functor .
    template <await_functor F>
    struct task_awaitable
    {
        inline bool await_ready()
        {
            return F::isResumable();
        }

        template <task_promise T>
        inline void await_suspend(std::coroutine_handle<T> h)
        {
            h.promise().isResumable = F::isResumable;
        }

        inline auto await_resume()
        {
            return F::getResult();
        }
    };

} // namespace emcoro
#endif
/// @file awaitable.hpp
/// @author Mattis Kieffer (mattis.kieffer@hotmail.de)
/// @brief contains the task_awaitable template.
/// @version 1.0
/// @date 2023-01-02
/// @copyright Copyright (c) 2023
