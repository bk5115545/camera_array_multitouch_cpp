
#include "MotionProcessor.h"

#include <math.h>
#include <chrono>
#include <queue>
#include <algorithm>

std::shared_ptr<Frame> MotionProcessor::run(std::shared_ptr<Frame> f) {
	auto start = std::chrono::system_clock::now();
	
	cv::Mat temp = f->getData();

	if (f->getID() < first_frame_id) {
		first_frame_id = f->getID();
		f1 = temp;
		f2 = temp;
	}
	
	f2 = temp;

	cv::Mat out;
	
	cv::absdiff(f1, f2, out);
	cv::erode(out, out, cv::Mat(5, 5, CV_8UC1), cv::Point(0, 0));
	cv::bitwise_and(f->getData(), out, out);
	
	//out.convertTo(out, out.type(), 5.0, 0.0);

	cv::Vec3b zero(10, 10, 10);
	cv::Point tl(out.size().width, out.size().height);
	cv::Point br(0, 0);

	for (int col = 0; col < out.size().width; col++) {
		for (int row = 0; row < out.size().height; row++) {
			if (out.at<cv::Vec3b>(row, col) > zero) {
				// Update the top-left corner.
				if (col < tl.x) tl.x = col;
				if (row < tl.y) tl.y = row;

				// Update the bottom-right corner.
				if (col > br.x) br.x = col;
				if (row > br.y) br.y = row;
			}
		}
	}
	
	cv::rectangle(out, tl, br, cv::Scalar(0, 55, 255));

	f1 = f2;
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";

	return std::make_shared<Frame>(out, f->getCameraID(), f->getID());
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