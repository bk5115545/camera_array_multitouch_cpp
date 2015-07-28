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

	bool hasElement(int index) {
		try {
			cache.at(index);
			return true;
		}
		catch (std::out_of_range e) { return false; }
	}

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

	typedef std::pair<std::shared_ptr<Frame>, std::shared_ptr<Frame>> __previous_current__;
	static std::map<int, __previous_current__> cache; //i want this to be std::uniq_ptr<__previous_current__> but error

};