#pragma once

#include <memory>
#include <atomic>
#include <queue>

#include <opencv2/nonfree/features2d.hpp>

#include "CameraDevice.h"
#include "Processor.h"

class CalibrationProcessor : Processor {
	std::shared_ptr<Frame> frame;
	std::deque<CameraDevice> camera_locations;
	
	std::shared_ptr<cv::FeatureDetector> sift_detector;
	std::vector<cv::KeyPoint> historic_features;

private:
	// Calibration Functions
	void calibrateLens();
	void calibratePosition();

	// Helpers
	std::vector<cv::KeyPoint> getKeypoints();
	void filterKeypoints(std::vector<cv::KeyPoint> kp);

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};