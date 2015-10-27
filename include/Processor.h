#pragma once

#include <memory>

#include "Frame.h"
#include "FrameCache.h"

class Processor {

public:
	FrameCache * input_cache;
	FrameCache * output_cache;

	virtual void run() = 0;
	virtual void setCached(FrameCache * input_cache, FrameCache * output_cache) = 0;
	
	virtual FrameCache * getOutputCache() {
		return output_cache;
	}

	virtual FrameCache * getInputCache() {
		return input_cache;
	}
};