#pragma once

#include <memory>
#include <atomic>
#include <deque>

#include <opencv2/nonfree/features2d.hpp>

#include "CameraDevice.h"
#include "Processor.h"

class CalibrationProcessor : Processor {

	std::deque<CameraDevice> camera_locations;
	cv::SiftFeatureDetector sift_detector;

private:
	void calibrateLens(std::shared_ptr<Frame> frame);
	std::shared_ptr<Frame> calibratePosition(std::shared_ptr<Frame> frame);
	void calibrateCameraValue(int camera_id, int opencv_param_id, double opencv_param_value);

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};