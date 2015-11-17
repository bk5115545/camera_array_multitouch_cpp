
#include "FrameCache.h"

FrameCache::FrameCache(int max_size) {
	_cache.max_size = max_size;
}

void FrameCache::cache(std::shared_ptr<Frame> f) {
	_cache.push(f);
}

void FrameCache::empty() {
	_cache.empty();
}

std::shared_ptr<Frame> FrameCache::pop() {
	std::shared_ptr<Frame> f;

	_cache.wait_until_not_empty();
	_cache.try_pop(f);

	return f;
}

std::shared_ptr<Frame> FrameCache::get(int index) {
	std::shared_ptr<Frame> f;
	
	_cache.wait_until_not_empty();
	_cache.peek(index, f);
	
	return f;
}

std::shared_ptr<Frame> FrameCache::get_nowait(int index) {
	std::shared_ptr<Frame> f;
	
	_cache.peek(index, f);

	return f;
}

