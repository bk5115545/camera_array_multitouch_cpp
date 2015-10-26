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

public:
	Transformer();
	~Transformer();

	int addProcessor(Processor * p);
	int addFrame(std::shared_ptr<Frame> job);
	
	int processFrames();

	std::shared_ptr<Frame> getResult();

	//int removeProcessor();

};