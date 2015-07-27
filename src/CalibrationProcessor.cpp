
#include <iostream>

#include "CalibrationProcessor.h"
#include "CameraDevice.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> frame) {
	std::cout << "Camera ID: " << frame->getCameraID() << "\n";
	calibrateCameraValue(frame->getCameraID(), 5, 20.0);

	return std::make_shared<Frame>(frame->getData(), frame->getCameraID(), frame->getID());
}

/*
	calibrateLens
*/
void CalibrationProcessor::calibrateLens(std::shared_ptr<Frame> frame) {

}

/*
	calibratePosition
*/
void CalibrationProcessor::calibratePosition(std::shared_ptr<Frame> frame) {
	int camera_id = frame->getCameraID();
}

void CalibrationProcessor::calibrateCameraValue(int camera_id, int opencv_param_id, double opencv_param_value) {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[camera_id];
	camera->setOpenCVProperty(opencv_param_id, opencv_param_value);
}