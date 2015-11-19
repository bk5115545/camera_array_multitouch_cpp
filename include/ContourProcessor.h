#pragma once

#include <map>

#include "Processor.h"

class ContourProcessor: public Processor {

private:
	std::map<int,std::pair<cv::Point,cv::Point>> trackedParameters;


public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);

	void setTrackingID(int id,cv::Point hsl_low,cv::Point hsl_high);
};