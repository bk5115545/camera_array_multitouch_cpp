#pragma once

#include <memory>

#include "Frame.h"
#include "FrameCache.h"

class Processor {

protected:
	std::shared_ptr<Frame> current_frame;

	// Functions to implement
	virtual void computeFrame() = 0;

private:
	bool isRunning = true;

public:
	FrameCache * input_cache;
	FrameCache * output_cache;

	virtual void run() {
		while (isRunning) {
			current_frame = input_cache->get(0);

			if (current_frame->getCameraID() > -1)
				computeFrame();
		}
	}
	
	virtual void stop() {
		isRunning = false;
	}
};