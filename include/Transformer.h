#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "SharedConcurrentQueue.h"
#include "Frame.h"
#include "FrameCache.h"
#include "Processor.h"

class Transformer {
private:
	FrameCache * input_cache;
	FrameCache * output_cache;

	std::vector<Processor *> processors;
	int current_processor = 0;

public:
	Transformer();
	~Transformer();

	int addProcessor(Processor * p);
	int addFrame(std::shared_ptr<Frame> job);
	
	int processFrames();

	std::shared_ptr<Frame> getResult();
};