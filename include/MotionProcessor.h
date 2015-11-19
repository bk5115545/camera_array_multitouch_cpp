
#pragma once

#include "Processor.h"
#include "ImageProc.h"

class MotionProcessor : public Processor {

private:
	bool first_frame = true;
	cv::Mat previous_mat;

public:
	void computeFrame(std::shared_ptr<Frame> current_frame);
};