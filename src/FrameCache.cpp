
#include "FrameCache.h"

void FrameCache::cache(std::shared_ptr<Frame> f) {
	_cache.push(f);
}

std::shared_ptr<Frame> FrameCache::get(int index) {
	std::shared_ptr<Frame> f;

	_cache.wait_until_not_empty();

	if (_cache.peek(index, f))
		return f;
	else
		return std::make_shared<Frame>(Frame());
}

std::shared_ptr<Frame> FrameCache::get_nowait(int index) {
	std::shared_ptr<Frame> f;
	//_cache.wait_until_not_empty();
	return _cache.peek(index, f) ? f : std::make_shared<Frame>(Frame());
}

void FrameCache::empty() {
	_cache.empty();
}
