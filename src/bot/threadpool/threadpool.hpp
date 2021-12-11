//
// Created by X-ray on 8/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_THREADPOOL_HPP
#define DISCORD_CPP_BOT_THREADPOOL_HPP
#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

namespace bot {
    class thread_pool {
    public:
        using job_t = std::function<void()>;

        explicit thread_pool(int thread_count);

        ~thread_pool();

        void add_job(job_t job);

    private:
        std::vector<std::thread> threads_;
        std::queue<job_t> jobs_;
        std::mutex mutex_;
        std::atomic<bool> running_ = true;
    private:
        void run_tick();
    };
}
#endif //DISCORD_CPP_BOT_THREADPOOL_HPP
