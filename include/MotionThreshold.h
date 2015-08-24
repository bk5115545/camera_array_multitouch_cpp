
#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

#include "Frame.h"

class MotionThreshold {
	std::vector<cv::Mat> motions;
	std::vector<cv::Mat> history;

public:
	MotionThreshold();
	
	void pushMat(cv::Mat f);
	cv::Mat segmentMotion();

private:
	
};
