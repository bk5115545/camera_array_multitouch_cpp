#include <opencv2/opencv.hpp>

#include <thread>
#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

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
	CameraDevice dev(0);

	if (!dev.acquire()) return 1;

	Transformer tr (1);

	while (true) {
		//tr.enqueue(dev.getFrame());
		//cv::imshow("testing", tr.popResult()->operator cv::Mat());
		
		dev.setFps(20);
	//	std::cout << dev.getFps() << "\n";
		std::cout << dev.getResolution()[0] << "\n";

		//Frame * frame = dev.getFrame();
		//cv::imshow("testing", frame->operator cv::Mat);

		//if (cv::waitKey(27)) break;
	}

	return 0;
}
