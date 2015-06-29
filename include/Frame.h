#pragma once
#include <opencv2/opencv.hpp>

class Frame {

	private:
		cv::Mat _frame;
		int _camera_id;

	public:
		Frame(cv::Mat*, int camera_id);
		~Frame();

		cv::Mat* getData();
		int getCameraID();
};