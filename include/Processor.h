#pragma once

#include <memory>

#include "Frame.h"
#include "FrameCache.h"

class Processor {

public:
	FrameCache * input_cache;
	FrameCache * output_cache;

	virtual std::shared_ptr<Frame> run(std::shared_ptr<Frame>) = 0;
	virtual void setCached(FrameCache * input_cache, FrameCache * output_cache) = 0;
};