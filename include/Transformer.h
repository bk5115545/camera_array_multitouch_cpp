#pragma once

#include <vector>
#include <thread>

#include "Frame.h"
#include "FrameCache.h"
#include "Processor.h"

class Transformer {
private:
	FrameCache * input_cache;
	FrameCache * output_cache;

	std::vector<Processor *> processors;
	std::vector<std::thread> threads;

public:
	Transformer();
	~Transformer();

	void addProcessor(Processor * p);
	void addFrame(std::shared_ptr<Frame> job);
	
	void stopProcessors();

	std::shared_ptr<Frame> getResult();
};