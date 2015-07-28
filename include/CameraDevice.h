#pragma once
#include <opencv2/opencv.hpp>
#include <memory>

#include "Frame.h"

class CameraDevice {

	private:
		bool acquired = false;
		int camera_id = -1;
		int camera_position = 0;

		cv::VideoCapture capture;

	public:
		static std::vector<std::shared_ptr<CameraDevice>> devices;

		CameraDevice(int camera_id);
		~CameraDevice();

		bool acquire();
		void release();

		std::shared_ptr<Frame> getFrame();
		
		bool grabFrame();
		std::shared_ptr<Frame> decodeFrame(int channel=0);
		int getID();

		static std::shared_ptr<CameraDevice> getCameraDevice(int id);

		bool calibrate_lens();

		inline void setPosition(int value);
		inline int getPosition();

		inline double getFPS();
		inline bool setFPS(double value);

		inline std::pair<double, double> getResolution();
		inline std::pair<bool, bool> setResolution(double height, double width);

		double getOpenCVProperty(int prop_id);
		bool setOpenCVProperty(int prop_id, double value);
};