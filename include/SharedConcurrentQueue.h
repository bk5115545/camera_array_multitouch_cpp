#include <forward_list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <boost/timer.hpp>

template<typename Data> class concurrent_queue {
	static_assert(std::is_base_of<std::shared_ptr<Frame>, Data>::value,"ConcurrentQueue template arguement must be derrived from std::shared_ptr<Frame>");

	private:
		std::list<const Data> queue;
		boost::mutex queue_mutex;
		boost::condition_variable condition;

		const int max_size = 15;
		std::atomic<int> current_size = 0; //store size seperate so we can access it without locking the queue

	public:
		bool push(Data const& data) {
			if(current_size >= max_size) return false;

			boost::mutex::scoped_lock lock(queue_mutex);

			//check where to insert
			std::list<Data>::iterator index = queue.begin();
			if(current_size > 0) {
				while(index != queue.end() && index->get()->getID() < data->getID()) index++; //iterator search
			}

			queue.insert(index,data); //insert by iterator for O(1) instead of by index at O(n)
			current_size++;

			lock.unlock(); //unlock before notify

			condition.notify_one();
			return true;
		}

		bool empty() const {
			return current_size == 0;
		}

		bool try_pop(Data& popped_value) {
			boost::mutex::scoped_lock lock(queue_mutex);
			try {
				if(current_size > 0) {
					popped_value=queue.front();
					queue.erase(queue.begin());
					current_size--;
					return true;
				}
			} catch(std::exception e) {}
			return false;
		}

		void wait_and_pop(Data& popped_value) {
			boost::mutex::scoped_lock lock(queue_mutex);
			while(current_size == 0) {
				condition.wait(lock); //unlocks and waits.  re-locks on return
			}
			
			popped_value=queue.front();
			queue.erase(queue.begin());
			current_size--;
		}

};