
#pragma once

#include "Processor.h"
#include "ImageProc.h"

class MotionProcessor : public Processor {

private:
	bool first_frame = true;
	cv::Mat previous_mat;

	void calculateMotionMask();
	cv::Point getAveragePoint(cv::Mat & mat, std::vector<cv::Point> & nonzero_pts);
	bool findRGBPoint(std::vector<cv::Point> point_list, cv::Point to_find);

public:
	void computeFrame(std::shared_ptr<Frame> current_frame);
};