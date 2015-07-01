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
	frame_id %= 3600; //reset ID every minute
	return new Frame(&mat, camera_id, frame_id++);
}

/*
	This has been known in the community
	to not work, but I am testing it.
*/
double CameraDevice::getFps() {
	return capture.get(CV_CAP_PROP_FPS);
}

/*
	This has been known in the community
	to not work, but I am testing it.
*/
bool CameraDevice::setFps(double value) {
	return capture.set(CV_CAP_PROP_FPS, value);
}