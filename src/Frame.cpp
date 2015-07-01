#include "Frame.h"

Frame::Frame(cv::Mat* source, int camera_id, int frame_id) {
	source->assignTo(_frame);
	this->_camera_id = camera_id;
	this->_frame_id = frame_id;

	this->features = std::unordered_map<std::string, void*>();
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

int Frame::getID() {
	return _frame_id;
}

Frame::operator cv::Mat() const {
	return _frame;
}