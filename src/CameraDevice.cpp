#include "CameraDevice.h"

CameraDevice::CameraDevice(int camera_id)
	: camera_id(camera_id)
	, frame_id(0)
{
//	this->camera_id = camera_id;

//	this->frame_id = 0;
}

CameraDevice::~CameraDevice() {
	capture.release();
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
}

Frame* CameraDevice::getFrame() {
	cv::Mat mat;
	capture.read(mat);
	frame_id %= 3600; //reset ID every minute
	return new Frame(&mat, camera_id, frame_id++); // ::TODO:: Trace for memory leak
}