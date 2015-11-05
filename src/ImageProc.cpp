
#include "ImageProc.h"

ImageProc::ImageProc() {

}

ImageProc::~ImageProc() {

}

int ImageProc::absdiff(cv::Mat current, cv::Mat previous, cv::Mat & result) {
	for (int j = 0; j < current.rows; j++) {
		for (int i = 0; i < current.cols; i++) {
			result.at<cv::Vec3b>(cv::Point(i, j)) = current.at<cv::Vec3b>(cv::Point(i, j)) - previous.at<cv::Vec3b>(cv::Point(i, j));
		}
	}
	return 0;
}