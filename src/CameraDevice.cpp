#include "CameraDevice.h"

CameraDevice::CameraDevice(int camera_id) {
	this->camera_id = camera_id;
}

CameraDevice::~CameraDevice() {
	capture.release();
	capture = 0;
}

bool CameraDevice::acquire() {
	if (acquired) return true;

	capture = cv::VideoCapture(camera_id);
	if (capture.isOpened()) {
		acquired = true;
		return true;
	}

	return false;
}

void CameraDevice::release() {
	if (!acquired) return;

	acquired = false;
	capture.release();
	capture = 0;
}

Frame* CameraDevice::getFrame() {
	cv::Mat mat;
	capture.read(mat);
	return new Frame(&mat, camera_id);
}