
#pragma once

#include "Processor.h"
#include "ImageProc.h"

class MotionProcessor : public Processor {

// Private members

bool first_frame = true;
	
cv::Mat current_mat;
cv::Mat previous_mat;

void calculateMotionMask();

// Public methods
public:
	void run();

// Private methods
private:
	cv::Point getAveragePoint(cv::Mat & mat, std::vector<cv::Point> & nonzero_pts);
	bool findRGBPoint(std::vector<cv::Point> point_list, cv::Point to_find);
};