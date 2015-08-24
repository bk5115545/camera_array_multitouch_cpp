
#include "MotionProcessor.h"

#include <math.h>

MotionProcessor::MotionProcessor() {}

std::shared_ptr<Frame> MotionProcessor::run(std::shared_ptr<Frame> f) {
	cv::Mat temp = f->getData();
	cvtColor(f->getData(), temp, CV_BGR2GRAY);

	if (f->getID() < first_frame_id) {
		first_frame_id = f->getID();
		f1 = temp;
		f2 = temp;
	}
	
	f2 = temp;

	cv::Mat out;
	cv::absdiff(f1, f2, out);
	cv::medianBlur(out, out, 3);

	cv::cvtColor(out, out, CV_GRAY2RGB);
	cv::Mat new_out = cv::Mat::zeros(f->getData().size(), f->getData().type());
	cv::bitwise_and(f->getData(), out, new_out);

	f1 = f2;

	return std::make_shared<Frame>(new_out, f->getCameraID(), f->getID());
}