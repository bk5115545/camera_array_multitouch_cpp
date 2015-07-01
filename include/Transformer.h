#pragma once

#include <vector>
#include <thread>
#include <boost/lockfree/queue.hpp>
#include "Frame.h"

class Transformer {

	private:
		boost::lockfree::queue<Frame*> jobs;
		boost::lockfree::queue<Frame*> results;

		static std::atomic<int> class_threads;

		std::atomic<int> instance_threads = 0;
		std::atomic<int> max_threads = 0;
		std::atomic<int> job_count = 0;
		
		std::vector<std::thread*> threads;

	public:
		Transformer(int thread_count = 4);
		~Transformer();

		void run();
		std::vector<Frame*> stop_threads();
		
		int totalTransformerThreads();
		int enqueue(Frame* frame);
		Frame* popResult(bool blocking = false);


};