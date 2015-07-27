#pragma once

#include <memory>
#include <atomic>

#include "Processor.h"

class CalibrationProcessor : Processor {

private:

public:
	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);
};