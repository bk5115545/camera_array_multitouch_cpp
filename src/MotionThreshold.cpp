
#include <ios>

#include "MotionThreshold.h"

MotionThreshold::MotionThreshold() {

}

void MotionThreshold::pushMat(cv::Mat f) {
	history.push_back(f);

}

/*
	Helper Functions
*/

cv::Mat MotionThreshold::segmentMotion() {
	cv::Mat out;

	if (history.size() > 1) {
		for (int i = 0; i < history.size(); i += 2) {
			cv::Mat first = history.at(i);
			cv::Mat second = history.at(i + 1);
			cv::absdiff(first, second, out);
		}
	}

	return out;
}