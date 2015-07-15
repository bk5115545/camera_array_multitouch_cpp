#include <forward_list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

template<typename Data> class concurrent_queue {
	static_assert(std::is_base_of<std::shared_ptr<Frame>, Data>::value,"ConcurrentQueue template arguement must be derrived from Frame");
	private:
		std::forward_list<Data> the_queue;
		boost::mutex the_mutex;
		boost::condition_variable the_condition_variable;

	public:
		void push(Data const& data) {
			boost::mutex::scoped_lock lock(the_mutex);
			the_queue.push_back(data);
			lock.unlock();
			the_condition_variable.notify_all();
		}

		bool empty() const {
			boost::mutex::scoped_lock lock(the_mutex);
			return the_queue.empty();
		}

		bool try_pop(Data& popped_value) {
			boost::mutex::scoped_lock lock(the_mutex);
			if(the_queue.empty()) {
				return false;
			}

			popped_value=the_queue.front();
			the_queue.erase(the_queue.begin());
			return true;
		}

		void wait_and_pop(Data& popped_value) {
			boost::mutex::scoped_lock lock(the_mutex);
			while(the_queue.empty()) {
				the_condition_variable.wait(lock);
			}

			popped_value=the_queue.front();
			the_queue.erase(the_queue.begin());
		}

};