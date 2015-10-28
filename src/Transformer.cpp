
#include "Transformer.h"

Transformer::Transformer() {
	input_cache = new FrameCache();
	output_cache = new FrameCache();
}

Transformer::~Transformer() {
	if (output_cache)
		delete output_cache;

	if (input_cache)
		delete input_cache;
}

/*
 * Should maybe be using a pointer to a pointer to make the chain explicit
*/
int Transformer::addProcessor(Processor * p) {
	p->setCached(input_cache, output_cache);
	processors.push_back(p);
	
	return 0;
}

int Transformer::addFrame(std::shared_ptr<Frame> job) {
	//std::cout << job->getID() << std::endl;
	input_cache->cache(job);
	return 0;
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