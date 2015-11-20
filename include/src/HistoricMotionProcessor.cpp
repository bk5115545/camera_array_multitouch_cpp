
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

	cv::Mat motion_mask = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_mask"));
	cv::Mat motion_locations = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_locs"));

	cv::Mat color_mm = cv::Mat(current_mat.size(), current_mat.type());

	for (int i = 0; i < motion_locations.rows; i++) {
		cv::Point loc_i = motion_locations.at<cv::Point>(i);
		color_mm.at<cv::Vec3b>(loc_i) = current_mat.at<cv::Vec3b>(loc_i);
	}

	//return std::make_shared<Frame>(color_mm, current_frame->getCameraID(), current_frame->getID());

	if (first_frame) {
		first_frame = false;
		previous_locs = motion_locations;
	}

	/*
		Blue - Previous Location
	*/
	for (int i = 0; i < previous_locs.rows; i++) {
		cv::Point loc_i = previous_locs.at<cv::Point>(i);
		blank_mat.at<cv::Vec3b>(loc_i) = cv::Vec3b(255, 0, 0);
	}
	
	/*
		Green - Current Location
	*/
	for (int i = 0; i < motion_locations.rows; i++) {
		cv::Point loc_i = motion_locations.at<cv::Point>(i);
		blank_mat.at<cv::Vec3b>(loc_i) = cv::Vec3b(0, 255, 0);
	}

	previous_locs = motion_locations;

	//return current_frame;
	return std::make_shared<Frame>(blank_mat, current_frame->getCameraID(), current_frame->getID());
}