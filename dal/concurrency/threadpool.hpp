/*
 * Modified by wegamekinglc on 2021/3/10.
 * Written by Antoine Savine in 2018
 * This code is the strict IP of Antoine Savine
 * License to use and alter this code for personal and commercial applications
 * is freely granted to any person or company who purchased a copy of the book
 * Modern Computational Finance: AAD and Parallel Simulations
 * Antoine Savine
 * Wiley, 2018
 * As long as this comment is preserved at the top of the file
 */

#pragma once

#include <dal/concurrency/concurrentqueue.hpp>
#include <dal/math/vectors.hpp>
#include <dal/platform/platform.hpp>
#include <future>
#include <thread>

namespace Dal {
    using Task_ = std::packaged_task<bool(void)>;
    using TaskHandle_ = std::future<bool>;

    class ThreadPool_ {
        static ThreadPool_ instance_;
        ConcurrentQueue_<Task_> queue_;
        Vector_<std::thread> threads_;
        bool active_;
        bool interrupt_;
        static thread_local size_t tlsNum_;

        void ThreadFunc(const size_t& num);
        //  The constructor stays private, ensuring single instance
        ThreadPool_() : active_(false), interrupt_(false) {}

    public:
        static ThreadPool_* GetInstance() { return &instance_; }

        size_t NumThreads() const { return threads_.size(); }

        static size_t ThreadNum() { return tlsNum_; }

        void Start(const size_t& nThread = std::thread::hardware_concurrency() - 1);

        ~ThreadPool_() { Stop(); }

        void Stop();

        ThreadPool_(const ThreadPool_& rhs) = delete;
        ThreadPool_& operator=(const ThreadPool_& rhs) = delete;
        ThreadPool_(ThreadPool_&& rhs) = delete;
        ThreadPool_& operator=(ThreadPool_&& rhs) = delete;

        template <class C_> TaskHandle_ SpawnTask(C_ c) {
            Task_ t(std::move(c));
            TaskHandle_ f = t.get_future();
            queue_.Push(move(t));
            return f;
        }

        /*
         * Run queued tasks synchronously
         * while waiting on a future,
         * return true if at least one task was run
         */
        bool ActiveWaite(const TaskHandle_& f);
    };
} // namespace Dal
