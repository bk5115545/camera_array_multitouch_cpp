
#pragma once

#include "Processor.h"

class MotionProcessor : public Processor {

// Private members
private:
	cv::Mat f1;
	cv::Mat f2;

	bool first_frame = true;

	// Public methods
public:
	void run();
	void setCached(FrameCache * input_cache, FrameCache * output_cache);

// Private methods
private:
	cv::Point getAveragePoint(cv::Mat & mat, std::vector<cv::Point> & nonzero_pts);
	bool findRGBPoint(std::vector<cv::Point> point_list, cv::Point to_find);


};