
#include "Transformer.h"

Transformer::Transformer() {
	input_cache = new FrameCache(2);
	output_cache = new FrameCache(2);
}

Transformer::~Transformer() {
	if (output_cache)
		delete output_cache;

	if (input_cache)
		delete input_cache;
}

void Transformer::addProcessor(Processor * p) {
	if (!processors.empty()) {
		p->setCached(processors.back()->getOutputCache(), output_cache);
		processors.push_back(p);
	} else {
		p->setCached(input_cache, output_cache);
		processors.push_back(p);
	}
}

void Transformer::addFrame(std::shared_ptr<Frame> job) {
	input_cache->cache(job);
}

int Transformer::processFrames() {
	for (Processor * proc : processors) {
		proc->run();
	}

	return 0;
}

std::shared_ptr<Frame> Transformer::getResult() {
	return output_cache->get_nowait(0);
}