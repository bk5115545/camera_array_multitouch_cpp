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
	Grabs the next frame but does not decode, useful
	for decoding for special cameras (such as depth or Kinect)
	or multi-camera environments

	decodeFrame needs to be called after this to decode.

	OUTPUT:
		false if unsuccessful
*/
bool CameraDevice::grabFrame() {
	return capture.grab();
}

/*
	Decodes a frame based on a channnel

	INPUT:
		An OpenCV Mat to store the decoded frame
		An int channel number

	OUTPUT:
		Frame* to the decoded frame
*/
Frame* CameraDevice::decodeFrame(int channel) {
	cv::Mat mat;
	capture.retrieve(mat, channel); // ::TODO:: Error Catching

	frame_id %= 3600; //reset ID every minute
	return new Frame(&mat, camera_id, frame_id++);
}

/*
	Gets the FPS of a camera

	::TODO:: Test on a video feed

	OUTPUT:
		Camera FPS as a double
*/
inline double CameraDevice::getFPS() {
	return capture.get(CV_CAP_PROP_FPS);
}

/*
	Sets the FPS of a camera

	::TODO:: Test on a video feed

	INPUT:
		FPS as a double

	OUTPUT:
		false if unsuccessful
*/
inline bool CameraDevice::setFPS(double value) {
	return capture.set(CV_CAP_PROP_FPS, value);
}

/*
	OUTPUT:
		set_values[0] false if get height is unsuccessful
		set_values[1] false if get width is unsuccessful
*/
inline std::pair<double, double> CameraDevice::getResolution() {
	return std::pair<double, double>(capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH));
}

/*
	INPUT:
		height as a double
		width as a double

	OUTPUT:
		err_flags[0] false if set height is unsuccessful
		err_flags[1] false if set width is unsuccessful
*/
inline std::pair<bool, bool> CameraDevice::setResolution(double height, double width) {
	return std::pair<bool, bool>(capture.set(CV_CAP_PROP_FRAME_HEIGHT, height), capture.set(CV_CAP_PROP_FRAME_WIDTH, width));
}

/*
	INPUT:
		http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get

	OUTPUT:
		0 if get was unsuccessful
*/
inline double CameraDevice::getOpenCVProperty(int prop_id) {
	return capture.get(prop_id); //::TODO:: Do error catching
}

/*
	INPUT:
		http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get

	OUTPUT:
		false if set was unsuccessful
*/
inline bool CameraDevice::setOpenCVProperty(int prop_id, double value) {
	return capture.set(prop_id, value);
}
