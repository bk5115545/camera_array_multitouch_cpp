#pragma once

#include <memory>

#include "Frame.h"
#include "FrameCache.h"

class Processor {

public:
	FrameCache * input_cache;
	FrameCache * output_cache;

	bool isRunning;

	virtual void run() = 0;
	
	virtual void stop() {
		isRunning = false;
	}

	virtual void setCache(FrameCache * input_cache, FrameCache * output_cache) {
		this->input_cache = input_cache;
		this->output_cache = output_cache;
	}
	
	virtual FrameCache * getOutputCache() {
		return output_cache;
	}

	virtual FrameCache * getInputCache() {
		return input_cache;
	}
};