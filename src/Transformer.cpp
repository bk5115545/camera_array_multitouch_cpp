
#include "Transformer.h"

Transformer::Transformer() {}

Transformer::~Transformer() {}

int Transformer::addProcessor(Processor * p) {
	processors.push_back(p);
	return 0;
}


int Transformer::addFrame(std::shared_ptr<Frame> job) {
	frames.push(job);
	return 0;
}

int Transformer::processFrames() {
	for (int i = 0; i < frames.size(); i++) {
		std::shared_ptr<Frame> frame;

		if (!frames.try_pop(frame))
			continue;

		for (int i = 0; i < processors.size(); i++) {
			frame = processors[i]->run(frame);
		}

		result_frames.push(frame);
	}

	return 0;
}

std::shared_ptr<Frame> Transformer::getResult() {
	std::shared_ptr<Frame> output = nullptr;

	if (result_frames.try_pop(output))
		return output; //return result

	return std::shared_ptr<Frame>(); //return nullptr
}