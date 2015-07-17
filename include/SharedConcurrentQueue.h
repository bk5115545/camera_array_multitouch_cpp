#include <forward_list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

template<typename Data> class concurrent_queue {
	static_assert(std::is_base_of<std::shared_ptr<Frame>, Data>::value,"ConcurrentQueue template arguement must be derrived from std::shared_ptr<Frame>");
	private:
		std::list<Data> the_queue;
		boost::mutex the_mutex;
		boost::condition_variable the_condition_variable;

		int max_size = 15;

	public:
		void push(Data const& data) {
			boost::mutex::scoped_lock lock(the_mutex);
			if(the_queue.size() > max_size) return;
			the_queue.push_back(data);
			the_queue.sort([](const Data & d1,const Data & d2) {
				return d1->getID() < d2->getID();
			});
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
			if(the_queue.size() > 0) {
				popped_value=the_queue.front();
				the_queue.erase(the_queue.begin());
			}
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