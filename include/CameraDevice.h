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

		double getFPS();
		bool setFPS(double value);

		std::pair<double, double> getResolution();
		std::pair<bool, bool> setResolution(double height, double width);

		double getOpenCVProperty(int prop_id);
		bool setOpenCVProperty(int prop_id, double value);
};