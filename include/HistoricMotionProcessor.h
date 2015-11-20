
#pragma once

#include "Processor.h"

class HistoricMotionProcessor : public Processor {
private:

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};