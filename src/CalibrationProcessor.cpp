
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;

	cv::Mat temp = calibratePosition();

	return std::make_shared<Frame>(temp, frame->getCameraID(), frame->getID());
}

/*
	calibrateLens
*/
void CalibrationProcessor::calibrateLens() {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[frame->getCameraID()];

	std::cout << camera->getOpenCVProperty(CV_CAP_PROP_CONTRAST) << "\n";
	camera->setOpenCVProperty(CV_CAP_PROP_CONTRAST, 30);
}

/*
	calibratePosition
*/
cv::Mat CalibrationProcessor::calibratePosition() {
	auto start = std::chrono::system_clock::now();

	/*
		Set Up Temporary Mat Container
	*/
	cv::Mat temp = frame->getData();
	cv::GaussianBlur(temp, temp, cv::Size(1, 1), 0.0, 0.0, cv::BORDER_DEFAULT);
	cvtColor(temp, temp, CV_BGR2GRAY);

	/*
		Initial frame
	*/
	if (frame->getID() < first_frame_id | frame->getID() % 3 < 1.0) {
		first_frame_id = frame->getID();
		first_frame = temp.clone();
	}

	/*
		Background Subtraction via Running Average and Thresholding
	*/
	cv::absdiff(first_frame, temp, temp);
	cv::erode(temp, temp, cv::Mat(), cv::Point(0, 0), 2, 1);
	cv::dilate(temp, temp, cv::Mat(1, 1, CV_8UC1), cv::Point(0, 0), 2, 1, 1);
	cv::threshold(temp, temp, 25, 255, CV_THRESH_BINARY);

	/*
		Simple Contour Detection
	*/
	cv::vector<cv::vector<cv::Point>> edges;
	cv::vector<cv::Vec4i> hierarchy;
	cv::findContours(temp, edges, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	std::cout << edges.size() << "\n";

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	//std::cout << duration << "\n";

	return temp;
}

/*
 *
 *	Helper Functions
 *
 */

