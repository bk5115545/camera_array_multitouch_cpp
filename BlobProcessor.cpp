#include "BlobProcessor.h"

BlobProcessor::BlobProcessor() {

}

std::shared_ptr<Frame> BlobProcessor::run(std::shared_ptr<Frame> frame) {
	if(previous.get() == nullptr) {
		previous = frame;
		return std::shared_ptr<Frame>(); //return nullptrn 
	}

	if(current.get() == nullptr) {
		current = frame;
		return std::shared_ptr<Frame>(); //return nullptr
	}

	cv::Mat tmp1, tmp2, result;

	cv::absdiff(previous->getData(),frame->getData(),tmp1);
	cv::absdiff(current->getData(),frame->getData(),tmp2);
	cv::bitwise_and(tmp1,tmp2,result);

	cv::threshold(result,result,35,255,CV_THRESH_BINARY);

	previous = current;
	current = frame;

	return std::make_shared<Frame>(result, frame->getCameraID(), frame->getID());
}