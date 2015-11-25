
#include "HistoricMotionProcessor.h"

#include <iostream>

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

	cv::Vec3b avg_color;

	for (int i = 0; i < motion_locations.rows; i++) {
		cv::Point loc_i = motion_locations.at<cv::Point>(i);
		cv::Vec3b loc_color = current_mat.at<cv::Vec3b>(loc_i);

		color_mm.at<cv::Vec3b>(loc_i) = loc_color;
		
		for (int j = 0; j < 3; j++)
			avg_color[j] += loc_color[j];
	}

	std::cout << (int) avg_color[0] << " " << (int) avg_color[1] << " " << (int) avg_color[2] << std::endl;

	current_frame->addFeature("color motion_mask", color_mm);

	return std::make_shared<Frame>(color_mm, current_frame->getCameraID(), current_frame->getID());
	return current_frame;
}