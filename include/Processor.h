#pragma once

#include <memory>

#include "Frame.h"
#include "FrameCache.h"

class Processor {

protected:
	// Functions to implement
	virtual void computeFrame(std::shared_ptr<Frame> current_frame) = 0;

private:
	bool isRunning = true;

public:
	FrameCache * input_cache;
	FrameCache * output_cache;

	void run() {
		while (isRunning) {
			computeFrame(input_cache->pop());
		}
	}
	
	void stop() {
		isRunning = false;
	}
};