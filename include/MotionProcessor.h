
#pragma once

#include "Processor.h"

class MotionProcessor : public Processor {

	cv::Mat f1;
	cv::Mat f2;

	bool first_frame = true;

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

private:
	cv::Point getAveragePoint(cv::Mat & mat, std::vector<cv::Point> & nonzero_pts);
	bool findRGBPoint(std::vector<cv::Point> point_list, cv::Point to_find);
};