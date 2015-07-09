#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "SharedConcurrentQueue.h"
#include "Frame.h"

class Transformer {

	private:
		concurrent_queue<std::shared_ptr<Frame>> jobs;
		concurrent_queue<std::shared_ptr<Frame>> results;

		static std::atomic<int> class_threads;

		std::atomic<int> instance_threads;
		std::atomic<int> max_threads;
		std::atomic<int> job_count;

		bool running = true;
		
		std::vector<std::thread*> threads;

	public:
		Transformer(int);
		~Transformer();

		void run();
		std::vector<Frame*> stop_threads();
		
		int totalTransformerThreads();
		int enqueue(std::shared_ptr<Frame> frame);

		std::shared_ptr<Frame> popResult();

};