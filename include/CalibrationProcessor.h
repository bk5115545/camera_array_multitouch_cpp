#pragma once

#include <memory>
#include <atomic>
#include <queue>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "CameraDevice.h"
#include "Processor.h"

struct CalibrationParameters {
	int camera_id;

	bool found_features;

	cv::Mat background;
	unsigned long long background_id = 9999999999999;
};

struct Movement {
	cv::Point previous_point;
	cv::Point average_point;

	bool right = false;
	bool left = false;
	
	bool up = false;
	bool down = false;
};

class CalibrationProcessor : Processor {
	std::shared_ptr<Frame> frame;
	
	static std::deque<CameraDevice> camera_locations;

	CalibrationParameters camera_parameters;
	Movement camera_movement;


private:
	// Calibration Functions
	void calibrateLens();
	cv::Mat calibratePosition();

	// Helpers
	cv::Point updateAverageLocation(cv::Mat image);
	void determineDirection(Movement & movement);
	void subtractBackground(CalibrationParameters & parameters, cv::Mat & currentframe);

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};