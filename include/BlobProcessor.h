#pragma once

#include <memory>
#include <atomic>

#include "Frame.h"
#include "Processor.h"
#include "SharedConcurrentQueue.h"

class BlobProcessor : public Processor {
private:
	std::shared_ptr<Frame> findBlob(cv::Mat diff, std::shared_ptr<Frame> color);

public:
	~BlobProcessor();

	void run();
	void setCached(FrameCache * input_cache, FrameCache * output_cache);
};