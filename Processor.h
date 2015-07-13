#pragma once
#include <memory>
#include "Frame.h"

class Processor {

public:
	Processor();
	virtual std::shared_ptr<Frame> run(std::shared_ptr<Frame> f) = 0;

};