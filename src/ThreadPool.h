//
// Created by Stepan Usatiuk on 17.04.2023.
//

#ifndef SEMBACKUP_THREADPOOL_H
#define SEMBACKUP_THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

/// Thread pool
/**
 * Handles ctrl-c via Signals, but it is expected of tasks to also do so
 * Forwards exception messages to the provided handler
 */
class ThreadPool {
public:
    /// Constructs a thread pool
    /// \param onError     Callback function that is called when an exception happens when executing a task
    /// \param workersNum  Amount of worker threads (default = number of cpu threads)
    ThreadPool(std::function<void(std::string)> onError, std::size_t workersNum = std::thread::hardware_concurrency());

    /// Destructor, instructs the threads to stop and joins them
    ~ThreadPool();

    /// Pushes a new task to the queue
    /// \param func Rvalue to the task functon
    void push(std::function<void()> &&func);

    /// Returns True if the queue is empty and there are no tasks running
    bool empty();

    std::mutex finishedLock;         ///< Lock to use when waiting on the finished variable
    std::condition_variable finished;///< Condition variable to wait for all tasks to finish

private:
    /// Thread loop
    void loop();

    std::queue<std::function<void()>> queue; ///< Task queue
    std::mutex queueLock;                    ///< Task queue lock
    std::condition_variable somethingNew;    ///< Condition variable to wait for new tasks
    std::vector<std::thread> threads;        ///< Vector of worker threads
    std::atomic<bool> stop = false;          ///< Stop signal for threads
    std::atomic<int> running = 0;            ///< Number of currently running tasks
    std::function<void(std::string)> onError;///< Function to call on exception in task
};

#endif//SEMBACKUP_THREADPOOL_H
