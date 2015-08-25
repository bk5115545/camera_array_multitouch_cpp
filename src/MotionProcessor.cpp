
#include "MotionProcessor.h"

#include <math.h>
#include <chrono>

std::shared_ptr<Frame> MotionProcessor::run(std::shared_ptr<Frame> f) {
	
	// Avg Time: 30ms

	cv::Mat temp = f->getData();
	cvtColor(f->getData(), temp, CV_BGR2RGB);

	if (f->getID() < first_frame_id) {
		first_frame_id = f->getID();
		f1 = temp;
		f2 = temp;
	}
	
	f2 = temp;

	cv::Mat out;
	cv::absdiff(f1, f2, out);
	cv::medianBlur(out, out, 3);
	
	cv::bitwise_and(f->getData(), out, out);

	f1 = f2;

	return std::make_shared<Frame>(out, f->getCameraID(), f->getID());
}