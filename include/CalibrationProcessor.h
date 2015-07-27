#pragma once

#include <memory>
#include <atomic>

#include "Processor.h"

class CalibrationProcessor : Processor {

private:
	void calibrateLens(std::shared_ptr<Frame> frame);
	void calibratePosition(std::shared_ptr<Frame> frame);
	void calibrateCameraValue(int camera_id, int opencv_param_id, double opencv_param_value);

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};