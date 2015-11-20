
#pragma once

#include "Processor.h"

class HistoricMotionProcessor : public Processor {
private:
	bool first_frame = true;
	cv::Mat previous_locs;

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};