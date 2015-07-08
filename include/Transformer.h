#pragma once

#include <vector>
#include <thread>

#include "concurrentqueue.h"
#include "Frame.h"

class Transformer {

	private:
		moodycamel::ConcurrentQueue<Frame> jobs;
		moodycamel::ConcurrentQueue<Frame> results;

		static std::atomic<int> class_threads;

		std::atomic<int> instance_threads;
		std::atomic<int> max_threads;
		std::atomic<int> job_count;
		
		std::vector<std::thread*> threads;

	public:
		Transformer(int);
		~Transformer();

		void run();
		std::vector<Frame*> stop_threads();
		
		int totalTransformerThreads();
		int enqueue(Frame&& frame);

		bool popResult(Frame*& into);

};