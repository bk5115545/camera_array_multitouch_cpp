#include "CameraDevice.h"
#include "vld.h"

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


/*
Returns a Frame pointer to a frame allocated on the heap.
You must ensure that this pointer is deleted.
*/
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
inline double CameraDevice::getFPS() {
	return capture.get(CV_CAP_PROP_FPS);
}

/*
	This has been known in the community
	to not work, but I am testing it.
*/
inline bool CameraDevice::setFPS(double value) {
	return capture.set(CV_CAP_PROP_FPS, value);
}

/*
	OUTPUT:
		set_values[0] true if get height is successful
		set_values[1] true if get width is successful
*/
inline std::pair<double, double> CameraDevice::getResolution() {
	return std::pair<double, double>(capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH));
}

/*
	OUTPUT:
		err_flags[0] true if set height is successful
		err_flags[1] true if set width is successful
*/
inline std::pair<bool, bool> CameraDevice::setResolution(double height, double width) {
	return std::pair<bool, bool>(capture.set(CV_CAP_PROP_FRAME_HEIGHT, height), capture.set(CV_CAP_PROP_FRAME_WIDTH, width));
}

inline double CameraDevice::getOpenCVProperty(int prop_id) {
	return capture.get(prop_id); //::TODO:: Do error catching
}

inline bool CameraDevice::setOpenCVProperty(int prop_id, double value) {
	return capture.set(prop_id, value);  //::TODO:: Do error catching
}