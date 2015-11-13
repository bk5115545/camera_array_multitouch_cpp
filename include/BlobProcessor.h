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
	void run();
};