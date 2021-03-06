
#pragma once

#include <opencv2/opencv.hpp>
#include <memory>

#include "Frame.h"

class CameraDevice {

	private:
		bool acquired = false;
		int camera_id = -1;
		int frame_id = 0;

		cv::VideoCapture capture;

		std::string window_name;

	public:
		CameraDevice(int camera_id);
		~CameraDevice();

		bool acquire();
		void release();

		std::shared_ptr<Frame> getFrame();
		
		bool grabFrame();
		std::shared_ptr<Frame> decodeFrame(int channel=0);
		int getID();

		std::string getName();

		inline double getFPS();
		inline bool setFPS(double value);

		inline std::pair<double, double> getResolution();
		inline std::pair<bool, bool> setResolution(double height, double width);

		inline double getOpenCVProperty(int prop_id);
		inline bool setOpenCVProperty(int prop_id, double value);
};