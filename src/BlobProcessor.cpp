#include "BlobProcessor.h"

std::map<int,BlobProcessor::__previous_current__> BlobProcessor::cache = std::map<int,__previous_current__>();
std::map<int,BlobProcessor::__previous_current__> BlobProcessor::last_result = std::map<int,__previous_current__>();
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


	cv::Mat oldest = cache[frame->getCameraID()].first->getData();
	cv::Mat recent = cache[frame->getCameraID()].second->getData();
	cv::Mat current = frame->getData();

	cv::Mat old_motion,recent_motion,result;

	//we probably need a blur here on the input frame to reduce camera noise.
	//cv::GaussianBlur(frame->getData(),frame->getData(),cv::Size(5,5),14,14);
	cv::medianBlur(frame->getData(), frame->getData(),3);
	cv::absdiff(oldest, current, old_motion);
	cv::absdiff(recent, current, recent_motion);
	cv::bitwise_and(old_motion, recent_motion, result);

	cache[frame->getCameraID()].first = cache[frame->getCameraID()].second;
	cache[frame->getCameraID()].second = frame;

	cv::cvtColor(result,result,cv::COLOR_BGR2GRAY);
	
	//return std::make_shared<Frame>(result, frame->getCameraID(), frame->getID());
	return findBlob(result,frame);
}

std::shared_ptr<Frame> BlobProcessor::findBlob(cv::Mat diff, std::shared_ptr<Frame> color) {
	cv::Scalar mean,stddev;
	cv::meanStdDev(diff,mean,stddev);
	if(stddev[0] < 1) {
		return std::shared_ptr<Frame>(nullptr);
	}

	//cv::Mat target = cv::Mat::zeros(diff.size(),diff.type());

	std::cout << color->getCameraID() << std::endl;

	
	return std::make_shared<Frame>(diff, color->getCameraID(), color->getID());
}