
#include <iostream>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> frame) {
	std::cout << "Camera ID: " << frame->getCameraID() << "\n";
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
	int camera_id = frame->getCameraID();
	//std::shared_ptr<CameraDevice> camera = CameraDevice::getCameraDevice(camera_id);

	std::vector<cv::KeyPoint> keypoints;

	cv::Mat output;

	sift_detector.detect(frame->getData(), keypoints);
	cv::drawKeypoints(frame->getData(), keypoints, output);

	std::cout << frame->getCameraID() << "\n";
	return std::make_shared<Frame>(output, frame->getCameraID(), frame->getID());
}

void CalibrationProcessor::calibrateCameraValue(int camera_id, int opencv_param_id, double opencv_param_value) {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[camera_id];
	camera->setOpenCVProperty(opencv_param_id, opencv_param_value);
}