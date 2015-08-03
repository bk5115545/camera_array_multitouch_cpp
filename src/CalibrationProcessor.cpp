
#include <iostream>
#include <chrono>

#include "CalibrationProcessor.h"

std::map<int, CalibrationProcessor::__previous_current__> CalibrationProcessor::cache = std::map<int, __previous_current__>();

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> frame) {
	sift_detector = std::make_shared<cv::SiftFeatureDetector>();
	
	auto t1 = std::chrono::high_resolution_clock::now();	
	cvtColor(frame->getData(), frame->getData(), CV_BGR2RGB);
	frame = calibratePosition(frame);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	return frame; //calibratePosition(frame);
}

/*
	calibrateLens
*/
void CalibrationProcessor::calibrateLens(std::shared_ptr<Frame> frame) {

}

/*
	calibratePosition
*/
std::shared_ptr<Frame> CalibrationProcessor::calibratePosition(std::shared_ptr<Frame> frame) {
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat output;

	auto t1 = std::chrono::high_resolution_clock::now();
	sift_detector->detect(frame->getData(), keypoints);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "\n";


	cv::drawKeypoints(frame->getData(), keypoints, frame->getData());

	//std::cout << frame->getCameraID() << "\n";

	return frame;
}

void CalibrationProcessor::calibrateCameraValue(int camera_id, int opencv_param_id, double opencv_param_value) {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[camera_id];
	camera->setOpenCVProperty(opencv_param_id, opencv_param_value);
}