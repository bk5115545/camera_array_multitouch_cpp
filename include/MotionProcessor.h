
#pragma once

#include "Processor.h"

#include <math.h>
#include <chrono>
#include <queue>
#include <algorithm>

class MotionProcessor : Processor {
private:
	cv::Mat current_frame;
	cv::Mat previous_frame;
	cv::Mat processed_frame;

	std::vector<cv::Mat> motion_masks;

	bool first_frame = true;
	
	void calculateMotionMask();

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);


};