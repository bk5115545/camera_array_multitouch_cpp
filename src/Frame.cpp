#include "Frame.h"

Frame::Frame() {
	_camera_id = -1;
	_frame_id = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Frame::Frame(cv::Mat source, int camera_id) {
	_frame = source;

	_camera_id = camera_id;
	_frame_id = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Frame::Frame(cv::Mat source, int camera_id, unsigned long long frame_id) {
	_frame = source;

	_camera_id = camera_id;
	_frame_id = frame_id;
}

cv::Mat Frame::getData() {
	return _frame;
}

int Frame::getCameraID() {
	return _camera_id;
}

unsigned long long Frame::getID() {
	return _frame_id;
}

bool Frame::operator==(Frame f1) {
	if (f1.getID() == _frame_id) {
		return true;
	}

	return false;
}