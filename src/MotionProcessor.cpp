
#include "MotionProcessor.h"

std::shared_ptr<Frame> MotionProcessor::run(std::shared_ptr<Frame> f) {
	auto start = std::chrono::system_clock::now();

	if (first_frame) {
		first_frame = false;
		previous_frame = f->getData();
	}

	current_frame = f->getData();
	//processed_frame = current_frame;
	
	cv::absdiff(previous_frame, current_frame,  processed_frame);

	//cv::erode(processed_frame, processed_frame, cv::Mat(5, 5, CV_8UC1), cv::Point(0, 0));
	//cv::bitwise_and(current_frame, processed_frame, processed_frame);

	calculateMotionMask();

	previous_frame = current_frame;

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";

	return std::make_shared<Frame>(processed_frame, f->getCameraID(), f->getID());
}

void MotionProcessor::calculateMotionMask() {
	cv::Mat canny_output;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	float thresh = 100;
	cv::Canny(processed_frame, canny_output, thresh, thresh * 2, 3);

	cv::findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	
	std::vector<std::vector<cv::Point>> contours_poly(contours.size());
	std::vector<cv::Rect> motion_rects;

	for (int i = 0; i < contours.size(); i++) {
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		motion_rects.push_back(cv::boundingRect(cv::Mat(contours_poly[i])));
	}

	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(100,0,0);
		rectangle(processed_frame, motion_rects[i].tl(), motion_rects[i].br(), color, 2, 8, 0);
	}
}