#pragma once
#include <opencv2/opencv.hpp>
#include <string>


class Frame {

	private:
		cv::Mat _frame;
		int _camera_id;
		int _frame_id;

	public:
		Frame();
		Frame(cv::Mat* source, int camera_id, int frame_id);

		cv::Mat getData();
		int getCameraID();
		int getID();

		operator cv::Mat() const;
};