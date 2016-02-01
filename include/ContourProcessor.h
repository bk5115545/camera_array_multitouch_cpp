#pragma once

#include <map>

#include "Processor.h"

class ContourProcessor: Processor {

private:
	std::map<int,std::pair<cv::Point,cv::Point>> trackedParameters;


public:
	ContourProcessor();
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

	void setTrackingID(int id,cv::Point hsl_low,cv::Point hsl_high);
};