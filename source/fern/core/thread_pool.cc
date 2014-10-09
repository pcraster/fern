#include "fern/core/thread_pool.h"
#include <cassert>
#include <iostream>


namespace fern {

//! Construct a thread pool.
/*!
    \param         nr_threads Number of threads to create.
    \warning       As soon as the pool is created, the threads will start
                   probing the work queue for work.
*/
ThreadPool::ThreadPool(
    size_t nr_threads)

    : _done(false),
      _joiner(_threads)

{
    assert(nr_threads > 0);

    try {
        for(size_t i = 0; i < nr_threads; ++i) {
            _threads.emplace_back(&ThreadPool::execute_task_or_wait, this);
            // _threads.emplace_back(&ThreadPool::execute_task_or_yield, this);
        }
    }
    catch(...) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _done = true;
        }
        _work_condition.notify_all();
        throw;
    }
}


//! Destruct the thread pool.
/*!
*/
ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _done = true;
    }
    _work_condition.notify_all();
}


//! Return the number of threads in the pool.
/*!
*/
size_t ThreadPool::size() const
{
    return _threads.size();
}


/// //! Function that is run by each thread in the pool.
/// /*!
///     If a task is available, it is executed. If not, the thread will yield.
/// 
///     This is useful if the work queue is hardly ever empty. All threads in the
///     pool will burn CPU cycles.
/// */
/// void ThreadPool::execute_task_or_yield()
/// {
///     while(!_done) {
/// 
///         detail::FunctionWrapper task;
/// 
///         if(_work_queue.try_pop(task)) {
///             // Allright! We actually have something useful to do.
///             task();
///         }
///         else {
///             // No tasks in the queue, so hint the implementation to allow
///             // other threads to run.
///             std::this_thread::yield();
///         }
///     }
/// }


//! Function that is run by each thread in the pool.
/*!
    If a task is available, it is executed. If not, the thread will wait for
    one to become available.

    This is useful if the work queue is empty regularly. Only the threads
    that are executing tasks are burning CPU cycles.
*/
void ThreadPool::execute_task_or_wait()
{
    while(!_done) {

        // Start by checking the queue for a task.
        detail::FunctionWrapper task;

        if(_work_queue.try_pop(task)) {
            task();
        }
        else {

            // Wait until a task got added to the queue, or someone decided
            // that we need to stop. Lock a mutex so all the waiting of
            // threads is synchronized. No two threads will be woken up
            // at the same time.
            // Unlock the mutex so multiple notifies by the condition
            // variable get handled ASAP.
            // Be careful not to start waiting while in fact we need to stop.
            // Don't start waiting if _done is already true!!! You will never
            // be woken up again...

            {
                std::unique_lock<std::mutex> lock(_mutex);
                if(!_done) {
                    _work_condition.wait(lock, [this] {
                        return !_work_queue.empty() || _done; });
                }
            }

            if(!_done) {

                detail::FunctionWrapper task;

                // Pick a task if there is one. We got woken up because a task
                // got added to the queue, but all threads start with checking
                // the queue for a task unconditionally. Our task may have been
                // taken already.
                if(_work_queue.try_pop(task)) {
                    task();
                }
            }
        }
    }
}

} // namespace fern