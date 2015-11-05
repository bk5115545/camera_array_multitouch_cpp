
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

	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}
}

void Transformer::addProcessor(Processor * p) {
	if (!processors.empty())
		p->setCache(processors.back()->getOutputCache(), output_cache);
	else
		p->setCache(input_cache, output_cache);

	threads.push_back(new std::thread([=] { p->run(); }));
	processors.push_back(p);
}

void Transformer::addFrame(std::shared_ptr<Frame> job) {
	input_cache->cache(job);
}

std::shared_ptr<Frame> Transformer::getResult() {
	return output_cache->get(0);
}