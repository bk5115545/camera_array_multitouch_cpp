#pragma once

#include "Processor.h"

class BlobProcessor: Processor {

public:
	BlobProcessor();

	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);

};