#include "BlobProcessor.h"

std::map<int,BlobProcessor::__previous_current__> BlobProcessor::cache = std::map<int,__previous_current__>();
BlobProcessor::BlobProcessor() {
	
}

std::shared_ptr<Frame> BlobProcessor::run(std::shared_ptr<Frame> frame) {
	
	if(!hasElement(frame->getCameraID())) {
		__previous_current__ pair;
		try {
			pair = cache.at(frame->getCameraID());
		}
		catch(std::out_of_range e) {
			pair = std::pair<std::shared_ptr<Frame>,std::shared_ptr<Frame>>();
			pair.first = frame;
			cache[frame->getCameraID()] = pair;
			
			return std::shared_ptr<Frame>(); //return nullptr
		}
		
	
	} 

	if(cache[frame->getCameraID()].second.get() == nullptr) {
		cache[frame->getCameraID()].second = frame;
		return std::shared_ptr<Frame>(); //return nullptr
	}


	cv::Mat tmp1, tmp2, result;

	//we probably need a blur here on the input frame to reduce camera noise.
	//cv::GaussianBlur(frame->getData(),frame->getData(),cv::Size(5,5),14,14);
	cv::medianBlur(frame->getData(),frame->getData(),3);
	cv::absdiff(cache[frame->getCameraID()].first->getData(),frame->getData(),tmp1);
	cv::absdiff(cache[frame->getCameraID()].second->getData(),frame->getData(),tmp2);
	cv::bitwise_and(tmp1,tmp2,result);

	cache[frame->getCameraID()].first = cache[frame->getCameraID()].second;
	cache[frame->getCameraID()].second = frame;

	return std::make_shared<Frame>(result, frame->getCameraID(), frame->getID());
}