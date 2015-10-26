
#include "FrameCache.h"

FrameCache::FrameCache() {

}

void FrameCache::cache(std::shared_ptr<Frame> f) {
	_cache.push(f);
}

std::shared_ptr<Frame> FrameCache::get(int index) {
	std::shared_ptr<Frame> f;
	
	if(_cache.peek(index, f))
		std::cout << f->getID() << std::endl;

	return f;
}

void FrameCache::empty() {
	_cache.empty();
}

std::pair<Frame, Frame> FrameCache::getRecent() {
	std::pair<Frame, Frame> history;

	std::shared_ptr<Frame> recent;
	if (!_cache.try_pop(recent)) {

	}

	std::shared_ptr<Frame> sec_recent;
	if (!_cache.try_pop(sec_recent)) {

	}

	history.first = *recent;
	history.second = *sec_recent;

	return history;
}