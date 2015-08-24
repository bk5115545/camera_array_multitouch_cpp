
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
	cv::threshold(out, out, 50, 255, CV_THRESH_BINARY);

	f1 = f2;

	std::vector<cv::Point> blah;
	if (cv::countNonZero(out) > 0) {
		cv::findNonZero(out, blah);
	}

	std::cout << blah.size() << "\n";

	// Convert Blah to cv::Mat (Binary) and then render blah

	return std::make_shared<Frame>(out, f->getCameraID(), f->getID());
}

void MotionProcessor::drawMotion() {

}