#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "SharedConcurrentQueue.h"
#include "Frame.h"
#include "Processor.h"

template <class Proc>
class Transformer {
	static_assert(std::is_base_of<Processor,Proc>::value,"Transformer template parameter must inherit from class Processor.");

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
		Transformer(int thread_limit) {
			max_threads = thread_limit;
			instance_threads = 0;
			job_count = 0;

			threads = std::vector<std::thread*>();
		}

		~Transformer() {}

		void run() {
			std::unique_ptr<Proc> processor = std::make_unique<Proc>(Proc());
			
			while(running) {
				std::shared_ptr<Frame> frame;
				jobs.wait_and_pop(frame);
				if(frame.get() == nullptr) continue;
				job_count--;
				std::shared_ptr<Frame> result = processor->run(frame);
				if(result.get() != nullptr) results.push(result);
			}

			class_threads--;
			instance_threads--;
			//this be my magic lambda of DOOOOMMMMMMMmmmmmm!
			threads.erase(std::find_if(threads.begin(),threads.end(),
				[=](std::thread* thread) {
				return std::this_thread::get_id() == thread->get_id();
			}));
			//don't touch it but it might be good to check that this thread hasn't been mistakenly deleted by another thread
			//because windows reuses thread_ids.  it's probably not possible but meh.  ::TODO:: check this
		}

		std::vector<Frame*> stop_threads() {
			running = false;

			return std::vector<Frame*>(0); //::TODO:: unimplemented -- needs to return list of unprocesse Frames
		}
		
		int totalTransformerThreads(){
			return class_threads;
		}
		int enqueue(std::shared_ptr<Frame> frame) {

			if(!jobs.push(frame)) {
				std::this_thread::sleep_for(std::chrono::microseconds(3));
				return job_count/ (instance_threads > 0 ? instance_threads.operator int() : 1);
			}

			job_count++;

			if(instance_threads < job_count && instance_threads < max_threads) {
				std::thread* newThread = new std::thread([=] { run(); });
				threads.push_back(newThread);
				instance_threads++;
				class_threads++;
			}

			return job_count/ (instance_threads > 0 ? instance_threads.operator int() : 1);
		}

		std::shared_ptr<Frame> popResult() {
			std::shared_ptr<Frame> output = nullptr;
			if(results.try_pop(output))
				return output; //return result
			return std::shared_ptr<Frame>(); //return nullptr
		}

};

template <class Proc>
std::atomic<int> Transformer<Proc>::class_threads = 0;