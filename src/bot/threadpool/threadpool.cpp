//
// Created by X-ray on 10/17/2021.
//

#include "threadpool.hpp"

namespace bot {
	thread_pool::thread_pool(int thread_count) {
		std::lock_guard lock(mutex_);
		for (int i = 0; i < thread_count; i++) {
			threads_.emplace_back([this]{
				while(this->running_) {
					this->run_tick();
				}
			});
		}
		threads_.shrink_to_fit();
	}

	thread_pool::~thread_pool() {
		running_ = false;

		// give threads time to shutdown
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		for (auto&& thread : threads_) {
			if (thread.joinable())
				thread.join();
		}
	}

	void thread_pool::add_job(job_t job) {
		try {
			if (job) {
				std::lock_guard lock(mutex_);
				jobs_.emplace(std::move(job));
			}
		} catch(...) {
		}
	}

	void thread_pool::run_tick() {
		try {
			job_t job = nullptr;
			{
				std::lock_guard lock(mutex_);
				if (!jobs_.empty()) {
					job = jobs_.front();
					jobs_.pop();
				}
			}
			if (job) {
				std::invoke(job);
			}
		} catch(...) {}
	}
}