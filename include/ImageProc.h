
#pragma once

#include <opencv2/opencv.hpp> // cv::Mat

class ImageProc {

public:
	static int absdiff(cv::Mat current, cv::Mat previous, cv::Mat & result);

private:

};