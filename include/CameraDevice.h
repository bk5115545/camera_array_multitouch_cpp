#pragma once
#include <opencv2/opencv.hpp>

#include "Frame.h"


class CameraDevice {

	private:
		bool acquired = false;
		int camera_id = -1;
		cv::VideoCapture capture;

	public:
		CameraDevice(int camera_id);
		~CameraDevice();

		bool acquire();
		void release();

		Frame* getFrame();

};