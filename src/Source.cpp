#include <opencv2/opencv.hpp>

#include <thread>
#include "CameraDevice.h"
#include "Frame.h"

int useSkinFinding(int camera_id) {
	cv::VideoCapture cap(camera_id); // open the default camera

	//CameraDevice device(camera_id);

	if (!cap.isOpened()) // check if we succeeded
		return -1;


	cv::namedWindow(std::string("Camera ") + std::to_string(camera_id));
	/*
	cv::namedWindow("Parameters", 1);
	cv::createTrackbar("H-Low", "Parameters", 0, 254, NULL);
	cv::createTrackbar("H-High", "Parameters", 0, 255, NULL);

	cv::createTrackbar("S-Low", "Parameters", 0, 254, NULL);
	cv::createTrackbar("S-High", "Parameters", 0, 255, NULL);

	cv::createTrackbar("V-Low", "Parameters", 0, 254, NULL);
	cv::createTrackbar("V-High", "Parameters", 0, 255, NULL);
	*/

	while (true) {
		
		cv::Mat frame;
		cv::vector<cv::vector<cv::Point>> contours;

		cap >> frame;
		cvtColor(frame, frame, CV_BGR2HSV);
		cv::inRange(frame, cv::Scalar(0, 48, 80), cv::Scalar(20, 255, 255), frame);
		//cv::inRange(frame, cv::Scalar(0, 49, 38), cv::Scalar(19, 121, 255), frame);
		//cv::inRange(frame, cv::Scalar(cv::getTrackbarPos("H-Low", "Parameters"), cv::getTrackbarPos("S-Low", "Parameters"), cv::getTrackbarPos("V-Low", "Parameters")), 
		//				   cv::Scalar(cv::getTrackbarPos("H-High", "Parameters"), cv::getTrackbarPos("S-High", "Parameters"), cv::getTrackbarPos("V-High", "Parameters")), frame);
		//cv::imshow("range", frame);

		//clean it up
		cv::dilate(frame, frame, cv::Mat(), cv::Point(-1, -1), 2, 1);
		cv::GaussianBlur(frame, frame, cv::Point(5,5), 14, 14);

		cv::erode(frame, frame, cv::Mat(), cv::Point(-1, -1), 3, 1);
		cv::dilate(frame, frame, cv::Mat(), cv::Point(-1, -1), 4, 1);
		cv::erode(frame, frame, cv::Mat(), cv::Point(-1, -1), 2, 1);
		//cv::imshow("cleanup", frame);

		//find edges
		cv::Mat edges;
		cv::Canny(frame, edges, 40, 100, 3, true);
		//cv::imshow("edge detect", edges);

		cv::RNG rng = cv::RNG(54121315467);
		cv::findContours(frame, contours, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_L1, cv::Point(0, 0));

		cv::Mat contourMat = cv::Mat::zeros(edges.size(), CV_8UC3);
		cv::Scalar color = cv::Scalar(255, 255, 255);
		cv::drawContours(contourMat, contours, -1, color, 2, 8);


		//cv::imshow("All Contours", contourMat);

		if (contours.size() > 0) {
			cv::vector<cv::vector<cv::Point>> interestingContours;

			for (unsigned int i = 1; i < contours.size(); i++) {
				printf("area:\t%f\n", cv::contourArea(contours[i]));
				if (cv::contourArea(contours[i]) > 10000) {
					interestingContours.push_back(contours[i]);
				}
			}

			cv::Mat interestingContour = cv::Mat::zeros(contourMat.size(), CV_8UC3);

			if (interestingContours.size() > 0) {
				cv::drawContours(interestingContour, interestingContours, -1, cv::Scalar(255, 0, 255), 2, 8);
			}

			cv::imshow(std::string("Camera ") + std::to_string(camera_id), interestingContour);
		}

		if (cv::waitKey(30) >= 0) break;
	}

	cv::destroyWindow(std::string("Camera ") + std::to_string(camera_id));
	return 1;
}


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
	std::thread t1(&useSkinFinding, 1);
	//std::thread t2(&useSkinFinding, 2); 
	std::thread t3(&useSkinFinding, 3); 
	//std::thread t4(&useSkinFinding, 4);

	t1.join();
	//t2.join();
	t3.join();
	//t4.join();

	return 0;
}
