
#pragma once

#include "SharedConcurrentQueue.h"
#include "Frame.h"

class FrameCache {

private:
	concurrent_queue<std::shared_ptr<Frame>> _cache;

public:
	FrameCache(int max_size);

	void empty();

	std::shared_ptr<Frame> pop();
	void push(std::shared_ptr<Frame> f);

	std::shared_ptr<Frame> get(int index);
	std::shared_ptr<Frame> get_nowait(int index);
};