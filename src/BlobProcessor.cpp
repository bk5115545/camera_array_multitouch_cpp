#include "BlobProcessor.h"


BlobProcessor::~BlobProcessor() {
	if (input_cache) delete input_cache;
	if (output_cache) delete output_cache;
}


void BlobProcessor::run() {
	//Frame recent = cache.getRecent().first;
	//Frame oldest = cache.getRecent().second;

	std::shared_ptr<Frame> frame = input_cache->get(0);
	output_cache->cache(frame);

	//cv::Mat recent_mat, oldest_mat;

//	cv::Mat current = frame->getData();
	
//	cv::Mat old_motion, recent_motion, result;

	//we probably need a blur here on the input frame to reduce camera noise.
//	cv::medianBlur(frame->getData(), frame->getData(),3);
//	cv::absdiff(oldest_mat, current, old_motion);
//	cv::absdiff(recent_mat, current, recent_motion);
//	cv::bitwise_and(old_motion, recent_motion, result);

//	cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
	
	
	//return findBlob(result, frame);

	return;
}

void BlobProcessor::setCached(FrameCache * input_cache, FrameCache * output_cache) {
	this->input_cache = input_cache;
	this->output_cache = output_cache;
}

std::shared_ptr<Frame> BlobProcessor::findBlob(cv::Mat diff, std::shared_ptr<Frame> color) {
	cv::Scalar mean,stddev;
	cv::meanStdDev(diff, mean, stddev);

	if(stddev[0] < 1) {
		return std::shared_ptr<Frame>(nullptr);
	}

	//cv::Mat target = cv::Mat::zeros(diff.size(),diff.type());
	
	return std::make_shared<Frame>(diff, color->getCameraID(), color->getID());
}