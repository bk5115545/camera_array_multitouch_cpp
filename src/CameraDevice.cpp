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

/*
	OUTPUT:
		set_values[0] true if get height is successful
		set_values[1] true if get width is successful
*/
double* CameraDevice::getResolution() {
	double size_values[2];

	size_values[0] = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	size_values[1] = capture.get(CV_CAP_PROP_FRAME_WIDTH);

	return size_values;
}

/*
	OUTPUT:
		err_flags[0] true if set height is successful
		err_flags[1] true if set width is successful
*/
bool* CameraDevice::setResolution(double height, double width) {
	bool err_flags[2];

	err_flags[0] = capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	err_flags[1] = capture.set(CV_CAP_PROP_FRAME_WIDTH, width);

	return err_flags;
}

double CameraDevice::getOpenCVProperty(int prop_id) {
	return capture.get(prop_id);
}

bool CameraDevice::setOpenCVProperty(int prop_id, double value) {
	return capture.set(prop_id, value);
}