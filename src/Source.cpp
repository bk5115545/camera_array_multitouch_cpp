#include <opencv2/opencv.hpp>

#include <thread>
#include "CameraDevice.h"
#include "Transformer.h"
//#include "Frame.h"

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
	CameraDevice dev(2);

	if (!dev.acquire()) return 1;

	Transformer tr(1);// = new Transformer(1);

	while (true) {
		Frame* inputFrame = dev.getFrame(); //implicit copy so we can get a new frame in the background
		std::cout << "c " << inputFrame->getCameraID() << " f " << inputFrame->getID()
			<< "\n";
		tr.enqueue(inputFrame); //implicit copy because we can't guarantee that the frame is available when it's processed

		Frame* frame = nullptr;

		if (tr.popResult(frame)) {
			cv::imshow("testing", frame->getData());
		}

		//if (cv::waitKey(27)) break;
	}

	return 0;
}
