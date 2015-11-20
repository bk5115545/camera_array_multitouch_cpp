#pragma once

#include "Processor.h"

class ContourProcessor: public Processor {

private:
	bool first_frame = true;
	cv::Mat previous_mat;

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};