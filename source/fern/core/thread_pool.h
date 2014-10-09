#pragma once
#include <condition_variable>
#include <future>
#include <thread>
#include <vector>
#include "fern/core/detail/function_wrapper.h"
#include "fern/core/join_threads.h"
#include "fern/core/thread_safe_queue.h"

#include <iostream>


namespace fern {

//! Thread pool.
/*!
    This thread pool contains a collection of threads that will pop tasks
    from a queue of tasks and execute them. In case there are no tasks
    to execute, threads will sleep until a new task is added to the queue.
    This means that it is OK to construct a ThreadPool instance without
    submitting tasks to it. The worker threads won't burn CPU cycles in
    that case.

    \sa            ThreadClient
*/
class ThreadPool
{

public:

                   ThreadPool          (size_t nr_threads);

                   ~ThreadPool         ();

    template<
        class Function>
    std::future<typename std::result_of<Function()>::type>
                   submit              (Function function);

    size_t         size                () const;

private:

    //! Variable to signal worker threads to stop executing new tasks.
    std::atomic_bool _done;

    //! Condition variable to block threads when there are not tasks.
    std::condition_variable _work_condition;

    //! Mutex used to protect access to _done and _work_condition.
    std::mutex     _mutex;

    //! Queue with tasks to execute.
    ThreadSafeQueue<detail::FunctionWrapper> _work_queue;

    //! Collection of worker threads.
    std::vector<std::thread> _threads;

    //! Utility member that will join all worker threads.
    JoinThreads    _joiner;

    /// void           execute_task_or_yield();

    void           execute_task_or_wait ();

};


//! Wrap the function or callable object in a task, push it on the work queue and return its future.
/*!
  \tparam    Function Type of function or callable object.
  \param     function Function or callable object to pass to thread pool.
  \return    Future holding the return value of the task, allowing the caller
             to wait for the task to complete.
*/
template<
    class Function>
inline std::future<typename std::result_of<Function()>::type>
    ThreadPool::submit(
        Function function)
{
    using result_type = typename std::result_of<Function()>::type;

    std::packaged_task<result_type()> task(std::move(function));
    std::future<result_type> result(task.get_future());
    _work_queue.push(std::move(task));

    // Notify a waiting thread (if there is any), that there is a new
    // task to perform.
    // We are not locking the mutex here. The thread that is being woken up
    // will try to acquire the lock and we don't want to keep her waiting.
    _work_condition.notify_one();

    return result;
}

} // namespace fern