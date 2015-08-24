
#pragma once

#include "Processor.h"
#include "MotionThreshold.h"

class MotionProcessor : Processor {

	cv::Mat f1;
	cv::Mat f2;

	unsigned long long first_frame_id = 9999999999999999999;

private:
	std::map<int, std::pair<cv::Point, cv::Point>> a;
	void drawMotion();

public:
	MotionProcessor();
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);
};