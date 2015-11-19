
#include "Transformer.h"

Transformer::Transformer() {
	input_cache = new FrameCache(2);
}

Transformer::~Transformer() {
	if (output_cache)
		delete output_cache;

	if (input_cache)
		delete input_cache;

	for (int i = 0; i < processors.size(); i++) {
		processors[i]->stop();
	}
}

void Transformer::addProcessor(Processor * p) {
	if (!processors.empty())
		p->input_cache = processors.back()->output_cache;
	else
		p->input_cache = input_cache;

	output_cache = new FrameCache(2);
	p->output_cache = output_cache;

	threads.push_back(std::thread([=] { p->run(); }));
	processors.push_back(p);
}

void Transformer::addFrame(std::shared_ptr<Frame> frame) {
	input_cache->push(frame);
}

std::shared_ptr<Frame> Transformer::getResult() {
	return output_cache->pop();
}