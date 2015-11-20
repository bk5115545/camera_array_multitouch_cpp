
#pragma once

#include "Processor.h"
#include "ImageProc.h"

class MotionProcessor : public Processor {

private:
	bool first_frame = true;

	cv::Mat previous_mat;
	cv::Mat current_mat;

	cv::Mat calculateMotionMask(std::shared_ptr<Frame> current_frame);

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};