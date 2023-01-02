#ifndef MKCORO_SCHEDULER_HPP
#define MKCORO_SCHEDULER_HPP
#include <tuple>
#include "task.hpp"

namespace emcoro
{
    /// @brief Round robin scheduler for coroutines.
    /// @tparam ...tasks deduced from constructor argument.
    template <class... tasks>
    class Scheduler
    {
        const std::tuple<tasks...> m_tasks;
        bool m_runnable{sizeof...(tasks) > 0};

        static bool pollTask(auto &task)
        {
            if (!task.m_handle || task.m_handle.done())
                return false;

            if (task.m_handle.promise().isResumable())
            {
                task.m_handle.resume();
            }
            return true;
        }

    public:
        Scheduler(tasks... args) : m_tasks{args...} {};

        void run()
        {
            while (m_runnable)
            {
                m_runnable = std::apply(
                    [](auto &&...a_tasks)
                    {
                        return (pollTask(a_tasks) | ...);
                    },
                    m_tasks);
            }
        }

        ~Scheduler()
        {
            std::apply(
                    [](auto &&...a_tasks)
                    {
                        (a_tasks.m_handle.destroy() , ...);
                    },
                    m_tasks);
        }
    };
};
#endif
/// @author Mattis Kieffer
/// @copyright