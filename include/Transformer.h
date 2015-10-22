#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "SharedConcurrentQueue.h"
#include "Frame.h"
#include "Processor.h"

class Transformer {
private:
	std::vector<Processor *> processors;

	concurrent_queue<std::shared_ptr<Frame>> frames;
	concurrent_queue<std::shared_ptr<Frame>> result_frames;

public:
	Transformer();
	~Transformer();

	int addProcessor(Processor * p);
	int addFrame(std::shared_ptr<Frame> job);
	
	int processFrames();

	std::shared_ptr<Frame> getResult();

	//int removeProcessor();
};