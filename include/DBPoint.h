
#pragma once

#include <opencv2/opencv.hpp>

class DBPoint {
public:
	cv::Point pt;
	bool visited;
	bool clustered;

	DBPoint();
	DBPoint(cv::Point pt);
};