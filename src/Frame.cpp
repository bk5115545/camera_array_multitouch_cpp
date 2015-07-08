#include "Frame.h"
#include "vld.h"

Frame::Frame() {
	_frame = cv::Mat::zeros(cv::Size(640, 480), 3);
	_camera_id = -1;
	_frame_id = -1;
} 

Frame::Frame(cv::Mat& source, int camera_id, int frame_id) {
	source.assignTo(_frame);
	//_frame = *source;
	this->_camera_id = camera_id;
	this->_frame_id = frame_id;
}


cv::Mat& Frame::getData() {
	return _frame;
}

int Frame::getCameraID() {
	return _camera_id;
}

int Frame::getID() {
	return _frame_id;
}
