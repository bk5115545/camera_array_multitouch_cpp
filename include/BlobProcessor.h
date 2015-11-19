#pragma once

#include "Processor.h"

class BlobProcessor : public Processor {
private:
	std::shared_ptr<Frame> findBlob(cv::Mat diff, std::shared_ptr<Frame> color);

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};