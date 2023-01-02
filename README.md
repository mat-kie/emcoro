# emcoro
A cooperative scheduler implementation using c++20 coroutines supporting awaitable functions and custom allocation.

# Notes
This is just a proof of concept / vaguely interesting code snippet showcasing a particular use case
for c++20 coroutines.

The idea is to create a cooparative scheduler with the c+20 coroutines feature
that does not use heap allocation (at least after initialization), allows proper suspension
of tasks on "blocking" calls (co_await) and an easy way to provide those awaitable functions.

since the co_await operator does not require the expression to be a coroutine, awaitable
funtions can be implemented by a very lightweigt wrapper of the return type that provides the
required member fuctions.

# usage

to declare a task just declare a function with the Task return type (see task.hpp).
when calling an awaitable function, use co_await.
(e.g. int x = co_await getInt(); instead of int x = getInt();)

to execute the tasks with the scheduler just call your Task-functions you implemented
in the schedulers initializer list and call scheduler::run() after that.