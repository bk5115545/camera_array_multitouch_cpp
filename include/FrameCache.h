
#pragma once

#include "SharedConcurrentQueue.h"
#include "Frame.h"

class FrameCache {

private:
	concurrent_queue<std::shared_ptr<Frame>> _cache;

public:
	FrameCache();
	FrameCache(int max_size);

	void cache(std::shared_ptr<Frame> f);
	void empty();

	std::shared_ptr<Frame> get(int index);
	std::shared_ptr<Frame> get_nowait(int index);
};