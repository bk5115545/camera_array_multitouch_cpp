#include <thread>
#include <memory>
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "BlobProcessor.h"
#include "ContourProcessor.h"

//#include "vld.h"

/*
int useMotionEstimation() {
	cv::VideoCapture cap(1);

	if (!cap.isOpened()) {
		return -1;
	}
	int* threshold_sensitivity = 0;
	cv::namedWindow("Threshold Settings");
	cv::createTrackbar("Sensitivity", "Threshold Settings", threshold_sensitivity, 255);

	cv::Mat previousFrame, currentFrame, liveFrame, tmpFrame, diffFrame;
	cap >> previousFrame;
	cv::cvtColor(previousFrame, previousFrame, cv::COLOR_BGR2GRAY);

	while (true) {
		cap.read(liveFrame);

		cv::cvtColor(liveFrame, currentFrame, cv::COLOR_BGR2GRAY);
		//cv::blur(...);
		cv::absdiff(previousFrame, currentFrame, diffFrame);
		cv::threshold(diffFrame, tmpFrame, cv::getTrackbarPos("Sensitivity", "Threshold Settings"), 255, cv::THRESH_BINARY);

		cv::imshow("Previous", previousFrame);
		cv::imshow("Current", currentFrame);
		cv::imshow("Difference", diffFrame);
		cv::imshow("Threshold", tmpFrame);

		previousFrame = currentFrame;

		if (cv::waitKey(30) >= 0) break;
	}

	cv::destroyAllWindows();
	return 1;
}
*/

int main(char* argsv, char argc) {
	CameraDevice dev(1);

	if (!dev.acquire()) return 1;
	
	Transformer<BlobProcessor> tr(4);
	cv::namedWindow("testing " + std::to_string(dev.getID()),1);

	while (true) {
		std::shared_ptr<Frame> inputFrame = dev.getFrame();
		
		int res = tr.enqueue(inputFrame);
				
		//std::cout << "FrameID:" << inputFrame->getID() << "\tJob Length:" << res << std::endl;

		std::shared_ptr<Frame> result = tr.popResult();
		
		if(result.get() != nullptr) {
			cv::imshow("testing " + std::to_string(dev.getID()),result->getData());
			if(cv::waitKey(10) >= 0) break;
		}
		
	}

	return 0;
}
