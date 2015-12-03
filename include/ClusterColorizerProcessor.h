
#pragma once

#include <array>

#include "Processor.h"

#include "DBScanProcessor.h"
#include "DBPoint.h"

class ClusterColorizerProcessor : public Processor {

private:
	std::array<cv::Vec3b, 10> colors;

public:
	ClusterColorizerProcessor();
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};