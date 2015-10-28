
#pragma once

#include "SharedConcurrentQueue.h"
#include "Frame.h"

class FrameCache {

private:
	int max_cache_size = 5;
	concurrent_queue<std::shared_ptr<Frame>> _cache;

public:
	FrameCache();

	void cache(std::shared_ptr<Frame> f);
	std::shared_ptr<Frame> get(int index);
	void empty();

	// first = most recent (not current), second = second recent
	std::pair<Frame, Frame> getRecent();
};