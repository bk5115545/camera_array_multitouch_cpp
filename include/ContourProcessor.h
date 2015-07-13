#pragma once

#include "Processor.h"

class ContourProcessor: Processor {

public:
	ContourProcessor();
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);
};