
#include "MotionProcessor.h"

#include <iostream>

void MotionProcessor::run() {
	while (true) {
		std::shared_ptr<Frame> frame = input_cache->get(0);
		output_cache->cache(frame);
	}
		/*
	std::shared_ptr<Frame> frame = input_cache->get(0);
	current_mat = frame->getData();
	
	if (first_frame) {
		first_frame = false;
		previous_mat = current_mat;
	}

	cv::Mat result_mat = current_mat;

	//cv::absdiff(current_mat, previous_mat, result_mat);
	//result_mat = cv::abs(current_mat - previous_mat);

	//cv::absdiff(previous_mat, current_mat, result_mat);
	
	//ImageProc::absdiff(current_mat, previous_mat, result_mat);
	
	//cv::log(current_mat, result_mat);

	//cv::max(current_mat, previous_mat, result_mat);

	frame->addFeature("differential mat", result_mat);
	output_cache->cache(frame);

	previous_mat = current_mat;
*/
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

void MotionProcessor::calculateMotionMask() {
	cv::Mat canny_output;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	float thresh = 100;
	//cv::Canny(processed_frame, canny_output, thresh, thresh * 2, 3);

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
	//	rectangle(processed_frame, motion_rects[i].tl(), motion_rects[i].br(), color, 2, 8, 0);
	}
}