
#include "Transformer.h"

Transformer::Transformer() {
	input_cache = new FrameCache(100);
}

/*
	TODO fix
*/
Transformer::Transformer(std::vector<Processor *> procs) {
	processors = procs;
}

/*
	TODO fix
*/
Transformer::Transformer(Processor * proc ...) {
	va_list args;
	
	va_start(args, proc);

	while (proc) {
		addProcessor(va_arg(args, Processor *));
		++proc;
	}
	
	va_end(args);
}

Transformer::~Transformer() {
	if (output_cache)
		delete output_cache;

	if (input_cache)
		delete input_cache;

	stopProcessors();
}

void Transformer::addProcessor(Processor * p) {
	if (!processors.empty())
		p->input_cache = processors.back()->output_cache;
	else
		p->input_cache = input_cache;

	output_cache = new FrameCache(100);
	p->output_cache = output_cache;

	threads.push_back(std::thread([=] { p->run(); }));
	processors.push_back(p);

	// Hack but at least it works for now
	threads[threads.size() - 1].detach();
}

void Transformer::addFrame(std::shared_ptr<Frame> frame) {
	input_cache->push(frame);
}

void Transformer::stopProcessors() {
	for (int i = 0; i < processors.size(); i++) {
		processors[i]->stop();
	}

	processors.clear();
}

std::shared_ptr<Frame> Transformer::getResult() {
	return output_cache->pop();
}