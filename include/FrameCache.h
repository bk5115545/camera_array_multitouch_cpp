
#pragma once

#include "SharedConcurrentQueue.h"
#include "Frame.h"

class FrameCache {

private:
	int max_cache_size = 5;
	concurrent_queue<std::shared_ptr<Frame>> _cache;

public:
	void cache(std::shared_ptr<Frame> f);
	void empty();

	std::shared_ptr<Frame> get(int index);
	std::shared_ptr<Frame> get_nowait(int index);
};