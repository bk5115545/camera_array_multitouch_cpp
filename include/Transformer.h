#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "SharedConcurrentQueue.h"
#include "Frame.h"
#include "Processor.h"

class Transformer {
private:
public:
	Transformer();
	~Transformer();
};