
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
	FrameCache* tmp = new FrameCache();
	p->setCached(output_cache, tmp);
	output_cache = tmp;
	processors.push_back(p);
	
	return 0;
}

int Transformer::addFrame(std::shared_ptr<Frame> job) {
	//std::cout << job->getID() << std::endl;
	input_cache->cache(job);
	return 0;
}

int Transformer::processFrames() {
	//std::shared_ptr<Frame> frame;
	//std::shared_ptr<Frame> result;
	//
	//for (int i = 0; i < frames.size(); i++) {
	//	if (!frames.try_pop(frame))
	//		continue;

	//	for (unsigned int j = 0; j < processors.size(); j++) {
	//		processors[j]->setCached(cache);
	//		result = processors[j]->run(frame);
	//	}

	//	cache.cacheFrame(frame);
	//	result_frames.push(frame);
	//}
	//

	return 0;
}

std::shared_ptr<Frame> Transformer::getResult() {
	std::shared_ptr<Frame> output = nullptr;

	//if (result_frames.try_pop(output))
	//	return output; //return result

	return std::shared_ptr<Frame>(); //return nullptr
}