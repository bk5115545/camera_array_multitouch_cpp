
#include "MotionProcessor.h"

#include <math.h>
#include <chrono>
#include <queue>
#include <algorithm>

std::shared_ptr<Frame> MotionProcessor::run(std::shared_ptr<Frame> f) {
	
	// Avg Time: 30ms
	auto start = std::chrono::system_clock::now();
	
	cv::Mat temp = f->getData();
	cvtColor(f->getData(), temp, CV_BGR2RGB);

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
	
	cv::cvtColor(out, out, CV_RGB2GRAY);
	std::vector<cv::Point> nonzero_pts;
	cv::Point avg_loc = getAveragePoint(out, nonzero_pts);

	if (avg_loc.x >= 0 && avg_loc.y >= 0) {
		floodMat(out, nonzero_pts, avg_loc);
	}

	f1 = f2;

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";

	return std::make_shared<Frame>(out, f->getCameraID(), f->getID());
}

std::vector<cv::Point> MotionProcessor::floodMat(cv::Mat & mat, std::vector<cv::Point> nonzero_pts, cv::Point origin) {
	std::queue<cv::Point> to_process;
	std::vector<cv::Point> hand_coords; 
	cv::Rect rect(cv::Point(), mat.size());

	to_process.push(origin);
	while (to_process.size() > 0) {
		cv::Point cur_loc = (cv::Point) to_process.front();
		to_process.pop();

		if (std::find(nonzero_pts.begin(), nonzero_pts.end(), cur_loc) != nonzero_pts.end()) {
			mat.at<uchar>(cur_loc) = 255;
			hand_coords.push_back(cur_loc);

			cv::Point north = cv::Point(cur_loc.x, cur_loc.y - 1);
			if (rect.contains(north))
				if (std::find(hand_coords.begin(), hand_coords.end(), north) != hand_coords.end())
					to_process.push(north);	// north
			
			cv::Point south = cv::Point(cur_loc.x, cur_loc.y + 1);
			if (rect.contains(south))
				if (std::find(hand_coords.begin(), hand_coords.end(), south) != hand_coords.end())
					to_process.push(south);	// south
			
			cv::Point east = cv::Point(cur_loc.x + 1, cur_loc.y);
			if (rect.contains(east))
				if (std::find(hand_coords.begin(), hand_coords.end(), east) != hand_coords.end())
					to_process.push(east);	// east
			
			cv::Point west = cv::Point(cur_loc.x - 1, cur_loc.y);
			if (rect.contains(west))
				if (std::find(hand_coords.begin(), hand_coords.end(), west) != hand_coords.end())
					to_process.push(west);	// west
		}

	}

	return hand_coords;
}

cv::Point MotionProcessor::getAveragePoint(cv::Mat & mat, std::vector<cv::Point> & nonzero_pts) {
	cv::Point avg;

	if (cv::countNonZero(mat) > 2000) {
		cv::findNonZero(mat, nonzero_pts);
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