#ifndef MKCORO_TASK_HPP
#define MKCORO_TASK_HPP
#include <coroutine>


namespace emcoro
{
    typedef bool (*pollingFunction_t)(void);

    template<typename Allocator>
    struct Task
    {
        class promise_type
        {
            static constexpr bool allwaysResume() { return true; }
        public:


            pollingFunction_t isResumable{allwaysResume};

            std::suspend_always yield_value(auto t) { return {}; }

            Task get_return_object() { return {std::coroutine_handle<promise_type>::from_promise(*this)}; }

            std::suspend_always initial_suspend() { return {}; }

            std::suspend_always final_suspend() noexcept { return {}; }

            void return_void() {}

            void unhandled_exception() {}

            void* operator new(size_t size){
                return Allocator::allocate(size);
            }


            void operator delete(void* mem){
                Allocator::deallocate(mem);
            }
        };

        std::coroutine_handle<promise_type> m_handle;
    };
};
#endif