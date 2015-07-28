
#include <iostream>

#include "CalibrationProcessor.h"

std::map<int, CalibrationProcessor::__previous_current__> CalibrationProcessor::cache = std::map<int, __previous_current__>();

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> frame) {

	if (!hasElement(frame->getCameraID())) {
		__previous_current__ pair;
		try {
			pair = cache.at(frame->getCameraID());
		}
		catch (std::out_of_range e) {
			pair = std::pair<std::shared_ptr<Frame>, std::shared_ptr<Frame>>();
			pair.first = frame;
			cache[frame->getCameraID()] = pair;

			return std::shared_ptr<Frame>(); //return nullptr
		}


	}

	if (cache[frame->getCameraID()].second.get() == nullptr) {
		cache[frame->getCameraID()].second = frame;
		return std::shared_ptr<Frame>(); //return nullptr
	}

	std::cout << "Camera ID: " << frame->getCameraID() << "\n";
	return calibratePosition(frame);
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

	sift_detector.detect(frame->getData(), keypoints);
	cv::drawKeypoints(frame->getData(), keypoints, output);

	std::cout << frame->getCameraID() << "\n";

	return std::make_shared<Frame>(output, frame->getCameraID(), frame->getID());
}

void CalibrationProcessor::calibrateCameraValue(int camera_id, int opencv_param_id, double opencv_param_value) {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[camera_id];
	camera->setOpenCVProperty(opencv_param_id, opencv_param_value);
}