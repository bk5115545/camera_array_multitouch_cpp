#include "Frame.h"

Frame::Frame(cv::Mat* source, int camera_id) {
	source->assignTo(_frame);
	this->_camera_id = camera_id;
}

Frame::~Frame() {
	_frame.release();
	_frame = 0;
}

cv::Mat* Frame::getData() {
	return &_frame;
}

int Frame::getCameraID() {
	return _camera_id;
}