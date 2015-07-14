#pragma once

#include <memory>

#include "Frame.h"
#include "Processor.h"

class BlobProcessor: Processor {
private:
	std::shared_ptr<Frame> previous;
	std::shared_ptr<Frame> current;

public:
	BlobProcessor();

	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};