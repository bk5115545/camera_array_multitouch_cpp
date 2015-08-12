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
};

class CalibrationProcessor : Processor {
	std::shared_ptr<Frame> frame;
	std::deque<CameraDevice> camera_locations;

	std::vector<CalibrationParameters> cameras_tested;

	cv::BackgroundSubtractorMOG bg = cv::BackgroundSubtractorMOG();
	
	cv::Mat average_frame;
	cv::Mat first_frame;

	unsigned long long number_of_frames = 0;

	unsigned long long first_frame_id = 9999999999999;

private:
	// Calibration Functions
	void calibrateLens();
	cv::Mat calibratePosition();

	// Helpers
	void updateAverageFrame(cv::Mat current_frame);

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};