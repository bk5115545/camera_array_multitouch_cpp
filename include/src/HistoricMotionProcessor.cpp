
#include "HistoricMotionProcessor.h"

/*
	HistoricMotionProcessor is responsible
	for deterimining motion over time whereas 
	MotionProcessor just computes a motion mask 
	in relation to the previous frame.

	Dependencies:
		MotionProcessor
*/

std::shared_ptr<Frame> HistoricMotionProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	cv::Mat current_mat = current_frame->getData();
	cv::Mat blank_mat = cv::Mat(current_mat.size(), current_mat.type());

	cv::Mat motion_locations = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_locs"));

	for (int i = 0; i < motion_locations.rows; i++) {
		cv::Point loc_i = motion_locations.at<cv::Point>(i);
		blank_mat.at<cv::Vec3b>(loc_i) = current_mat.at<cv::Vec3b>(loc_i);
	}
	
	//return current_frame;
	return std::make_shared<Frame>(blank_mat, current_frame->getCameraID(), current_frame->getID());
}