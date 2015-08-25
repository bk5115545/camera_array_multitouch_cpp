
#pragma once

#include "Processor.h"

class MotionProcessor : Processor {

	cv::Mat f1;
	cv::Mat f2;

	unsigned long long first_frame_id = 9999999999999999999;

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);
};