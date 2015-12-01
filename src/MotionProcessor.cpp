
#include "MotionProcessor.h"

/*
	MotionProcessor calculates the difference between the 
	current frame and the previous frame and stores the 
	result in the feature container labeled "diff"
*/

std::shared_ptr<Frame> MotionProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	current_mat = current_frame->getData();
	
	if (first_frame) {
		first_frame = false;
		previous_mat = current_mat;
	}

	// Calculate absolute difference
	cv::Mat diff_mat = cv::Mat(current_mat.size(), current_mat.type());
	
	cv::absdiff(current_mat, previous_mat, diff_mat);
	cv::erode(diff_mat, diff_mat, cv::Mat(5, 5, CV_8UC1), cv::Point(0, 0));
	cv::bitwise_and(current_mat, diff_mat, diff_mat);	// TODO do in full color

	current_frame->addFeature("diff", diff_mat);

	// Calculate motion mask
	cv::Mat motion_mask = cv::Mat(current_mat.size(), current_mat.type());
	cv::inRange(diff_mat, cv::Scalar(1, 1, 1), cv::Scalar(255, 255, 255), motion_mask);
	current_frame->addFeature("motion_mask", motion_mask);

	// Calculate non zero locations
	cv::Mat nonZeroLocs;
	cv::findNonZero(motion_mask, nonZeroLocs);	
	current_frame->addFeature("motion_locs", nonZeroLocs);

	// update previous frame
	previous_mat = current_mat;

	return current_frame;
}
