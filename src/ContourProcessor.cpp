#include "ContourProcessor.h"

/*
	Contour Processor searchs for the largest contour 
	and determines the average interior color
*/

std::shared_ptr<Frame> ContourProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	cv::Mat current_mat = current_frame->getData();
	cv::Mat color_mm = boost::any_cast<cv::Mat>(current_frame->getFeature("color motion_mask"));

	cv::vector<cv::vector<cv::Point>> contours;

	cv::Mat edges;
	cv::Canny(color_mm, edges, 0, 500, 3, false);

	cv::dilate(color_mm, color_mm, cv::Mat(), cv::Point(0, 0));

	cv::blur(color_mm, color_mm, cv::Size(3, 3));

	cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS, cv::Point(0, 0));
	
	cv::Mat contourMat = cv::Mat::zeros(edges.size(), CV_8UC3);
	cv::Scalar color = cv::Scalar(255, 255, 255);
	cv::drawContours(contourMat, contours, -1, color, 2, 8);

	current_frame->addFeature("contour mat", contourMat);

	return std::make_shared<Frame>(edges, current_frame->getCameraID(), current_frame->getID());
}
