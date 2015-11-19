
#include "BlobProcessor.h"

std::shared_ptr<Frame> BlobProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
//	Frame recent = cache.getRecent().first;
//	Frame oldest = cache.getRecent().second;
//	std::vector<int> blobs = { 1, 2, 3, 4 };
	
	cv::Mat inverse_mat = boost::any_cast<cv::Mat>(current_frame->getFeature("diff"));
	cv::bitwise_not(inverse_mat, inverse_mat);
	current_frame->addFeature("diff", inverse_mat);

	return std::make_shared<Frame>(inverse_mat, current_frame->getCameraID(), current_frame->getID());

//	output_cache->cache(frame);

//	cv::Mat recent_mat, oldest_mat;

//	cv::Mat current = frame->getData();
	
//	cv::Mat old_motion, recent_motion, result;

//	we probably need a blur here on the input frame to reduce camera noise.
//	cv::medianBlur(frame->getData(), frame->getData(),3);
//	cv::absdiff(oldest_mat, current, old_motion);
//	cv::absdiff(recent_mat, current, recent_motion);
//	cv::bitwise_and(old_motion, recent_motion, result);

//	cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
	
	
//	return findBlob(result, frame);
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