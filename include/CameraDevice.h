#pragma once
#include <opencv2/opencv.hpp>

#include "Frame.h"


class CameraDevice {

	private:
		bool acquired = false;
		int camera_id = -1;
		int frame_id = 0;
		cv::VideoCapture capture;

	public:
		CameraDevice(int camera_id);
		~CameraDevice();

		bool acquire();
		void release();

		Frame* getFrame();

		double getFps();
		bool setFps(double value);

};