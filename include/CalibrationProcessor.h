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

struct PositionCalibration {
	cv::Point previous_point;
	cv::Point center_of_mass;

	bool right = false;
	bool left = false;
	
	bool up = false;
	bool down = false;

	int last_known_position = 0; /* 0-right, 1-left, 2-top, 3-bottom */

	std::vector<int> loc_history;
};

struct LensCalibration {
	cv::Mat feature_image;

	std::vector<cv::KeyPoint> keypoints;
};

class CalibrationProcessor : Processor {
	std::shared_ptr<Frame> frame;
	
	static std::deque<CameraDevice> camera_locations;

	CalibrationParameters camera_parameters;
	PositionCalibration camera_movement;
	LensCalibration camera_lens;

private:
	// Calibration Functions
	void calibrateLens(cv::Mat & current_frame);
	void calibratePosition(cv::Mat & current_frame);

	// Helpers

	// Position Calibration
	void updateCenterOfMass(PositionCalibration & movement, cv::Mat & current_frame);
	void updateLastLocation(PositionCalibration & movement);
	void determineDirection(PositionCalibration & movement);
	void subtractBackground(CalibrationParameters & parameters, cv::Mat & current_frame);

	// Lens Calibration
	void extractFeatures(LensCalibration & lens, CalibrationParameters & parameters);

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};