
#pragma once

#include <array>

#include "Processor.h"

class ClusterColorizerProcessor : public Processor {
public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};