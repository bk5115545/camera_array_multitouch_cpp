
#include "MotionProcessor.h"

#include <math.h>
#include <chrono>
#include <queue>
#include <algorithm>

void MotionProcessor::run() {
	std::cout << "Processing motion" << std::endl;
	std::shared_ptr<Frame> frame = input_cache->get(0);
	output_cache->cache(frame);

//	auto start = std::chrono::system_clock::now();
	
//	cv::Mat temp = f->getData();

//	if (first_frame) {
//		first_frame = false;
//		f1 = temp;
//		f2 = temp;
//	}
	
//	f2 = temp;

//	cv::Mat out;
	
//	cv::absdiff(f1, f2, out);
//	cv::erode(out, out, cv::Mat(5, 5, CV_8UC1), cv::Point(0, 0));
//	cv::bitwise_and(f->getData(), out, out);

//	f1 = f2;
//	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
//		std::chrono::system_clock::now() - start).count();

//	std::cout << duration << "\n";

//	return std::make_shared<Frame>(out, f->getCameraID(), f->getID());
}

void MotionProcessor::setCached(FrameCache * input_cache, FrameCache * output_cache) {
	this->input_cache = input_cache;
	this->output_cache = output_cache;
}

cv::Point MotionProcessor::getAveragePoint(cv::Mat & mat, std::vector<cv::Point> & nonzero_pts) {
	cv::Point avg;

	if (nonzero_pts.size() > 0) {
		cv::Scalar avg_s_loc = cv::mean(nonzero_pts);

		avg.x = avg_s_loc[0];
		avg.y = avg_s_loc[1];

		cv::circle(mat, avg, 2, cv::Scalar(128, 128, 255));

		return avg;
	}

	avg.x = -1;
	avg.y = -1;
	
	return avg;	
}

bool MotionProcessor::findRGBPoint(std::vector<cv::Point> point_list, cv::Point to_find) {
	for (int i = 0; i < point_list.size(); i++) {
		if (point_list.at(i) == to_find)
			return true;
	}

	return false;
}