#include <string>

#include "Signals.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(std::function<void(std::string)> onError, std::size_t workersNum) : onError(std::move(onError)) {
    for (int i = 0; i < workersNum; i++) threads.emplace_back(&ThreadPool::loop, this);
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock(queueLock);
        stop = true;
        somethingNew.notify_all();
    }
    for (auto &t: threads) { t.join(); }
}

void ThreadPool::push(std::function<void()> &&func) {
    std::lock_guard lock(queueLock);
    queue.push(std::move(func));
    somethingNew.notify_one();
}

void ThreadPool::loop() {
    while (true) {
        std::unique_lock qLock(queueLock);

        while (queue.empty() && !stop && !Signals::shouldQuit) {
            // Check for any of the stop signals every second
            somethingNew.wait_for(qLock, std::chrono::seconds(1));
        }

        if (stop || Signals::shouldQuit) {
            // Drop all tasks if requested to exit
            queue = {};
            if (queue.empty() && running == 0) { finished.notify_all(); }
            return;
        }

        auto task = std::move(queue.front());

        running++;
        queue.pop();

        qLock.unlock();
        try {
            task();
        } catch (std::exception &e) { onError(std::string(e.what())); }
        qLock.lock();

        running--;
        if (queue.empty() && running == 0) { finished.notify_all(); }
    }
}

bool ThreadPool::empty() {
    std::lock_guard qLock(queueLock);
    if (queue.empty() && running == 0) return true;
    return false;
}
