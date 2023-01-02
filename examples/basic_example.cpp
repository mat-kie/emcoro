
#include <string_view>
#include <string>
#define MKDEBUG
#ifdef MKDEBUG
#include <iostream>
#endif
#include <emcoro/scheduler.hpp>
#include <emcoro/task.hpp>
#include <emcoro/awaitable.hpp>
#include <emcoro/resource.hpp>

using namespace emcoro;

struct AllocatorExceptionHandler
{
    static void fatal()
    {
        std::cout << "fatal exception";
        while (true)
            ;
    };
};

/// @brief Mock for a MMIO
class MyIoDriver
{

    static bool m_hasData;
    static char m_buffer[128];
    static size_t m_rx_size;

    static bool hasData()
    {
        if (m_hasData)
        {
            m_hasData = false;
            return true;
        };
        return m_hasData;
    }

    static std::string_view getData()
    {
        return std::string_view(m_buffer, m_rx_size);
    }

public:
    static void SetIO(std::string_view data)
    {
        m_rx_size = data.size() > 128 ? 128 : data.size();
        data.copy(m_buffer, m_rx_size);
        m_hasData = true;
    }

    /// @brief this is an awaitable function. it returns an awaitable object and
    /// can therefore be used in co_await expressions.
    /// @return
    static auto async_read()
    {
        // you could do initial setup for something here
        struct Awaitable
        {
            static bool isResumable()
            {
                // this function is evaluated to decide if the calling task should be resumed
                return hasData();
            };
            static auto getResult()
            {
                // this function is evaluated on resumption of the calling task
                return getData();
            }
        };

        return task_awaitable<Awaitable>{};
    }
};

bool MyIoDriver::m_hasData{false};
char MyIoDriver::m_buffer[128]{};
size_t MyIoDriver::m_rx_size{0};

// static allocator for coroutine frames of the tasks
using my_alloc = SequentialAllocator<AllocatorExceptionHandler, 1024>;

/// @brief Define a Task simply by declaring a function returning a task object.
/// @return
static Task<my_alloc> myTask()
{
#ifdef MKDEBUG
    std::cout << "start a Task as coroutine \n";
    // co_await suspends the task until the awaitable indicates that it should resume
    std::cout << "Coroutine resumed, async result: " << co_await MyIoDriver::async_read() << "\n";
    // this code only executes when the awaitable is ready.

#else
    co_await MyIoDriver::async_read();
#endif
    co_return;
}

static Task<my_alloc> myOtherTask()
{
    for (int i = 0; i < 10; ++i)
    {
#ifdef MKDEBUG
        std::cout << "\n my other task " << i << '\n';
#endif
        if (i == 4)
        {
            MyIoDriver::SetIO("an asynchronous message!");
        }
        // the Promise type has a simple yield_value function
        // just suspending the task irrespective of the argument.
        co_yield true;
    }
    co_return;
};

int main()
{
    // to evaluate Heap usage (or lack thereof) undefine MKDEBUG
    // create the Round-Robin Scheduler object to run the tasks
    Scheduler sched(myTask(), myOtherTask());
    // initialization phase done, lock the allocator
    my_alloc::lock();
    // start the scheduler
    sched.run();
}