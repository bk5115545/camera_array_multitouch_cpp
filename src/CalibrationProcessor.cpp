
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;
	
	// SIFT 400ms

	/*
		sift_detector = std::make_shared<cv::SiftFeatureDetector> (
			0,		// nFeatures
			4,		// nOctaveLayers
			0.04,	// contrastThreshold
			10,		// edgeThreshold
			1.6		// sigma
		);

		// Convert Frame to RGB
		cvtColor(frame->getData(), frame->getData(), CV_BGR2RGB);

		// Morph Frame
		auto start = std::chrono::system_clock::now();
		std::vector<cv::KeyPoint> keypoints;
		sift_detector->detect(frame->getData(), keypoints);
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now() - start).count();

		std::cout << duration << "\n";
		//calibrateLens();

		return frame;
	*/

	// BG Substraction 42ms 

	/*
		auto start = std::chrono::system_clock::now();
		cv::Mat cur = frame->getData();
		cv::Mat back;
		cv::Mat fore;

		bg.operator () (cur, fore);
		bg.getBackgroundImage(back);
		//cv::erode(fore, fore, cv::Mat());
		//cv::dilate(fore, fore, cv::Mat());

		Frame bg_frame = Frame(fore, frame->getCameraID());

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now() - start).count();

		std::cout << duration << "\n";

		return std::make_shared<Frame>(bg_frame);
	*/

	// Adaptive Thresholding 4ms

	auto start = std::chrono::system_clock::now();
	cv::Mat temp = frame->getData();

	cv::GaussianBlur(temp, temp, cv::Size(1, 1), 0.0, 0.0, cv::BORDER_DEFAULT);

	cvtColor(temp, temp, CV_BGR2GRAY);

	if (previous_frame.size() == cv::Size(0,0)) {
		previous_frame = frame->getData();
		cvtColor(previous_frame, previous_frame, CV_BGR2GRAY);

		std::cout << "Hello" << "\n";
	}

	cv::absdiff(previous_frame, temp, temp);
	cv::dilate(temp, temp, cv::Mat(1, 1, CV_8UC1), cv::Point(0, 0), 2, 1, 1);
	cv::threshold(temp, temp, 25, 255, CV_THRESH_BINARY);

	

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	//std::cout << duration << "\n";

	return std::make_shared<Frame>(temp, frame->getCameraID(), frame->getID());
}

/*
	calibrateLens
	*/
void CalibrationProcessor::calibrateLens() {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[frame->getCameraID()];

	std::cout << camera->getOpenCVProperty(CV_CAP_PROP_CONTRAST) << "\n";
	camera->setOpenCVProperty(CV_CAP_PROP_CONTRAST, 30);
}

/*
	calibratePosition
*/
void CalibrationProcessor::calibratePosition() {
	std::vector<cv::KeyPoint> keypoints = getKeypoints();

	cv::drawKeypoints(frame->getData(), keypoints, frame->getData());//, cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}

/*
 *
 *	Helper Functions
 *
 */

std::vector<cv::KeyPoint> CalibrationProcessor::getKeypoints() {
	std::vector<cv::KeyPoint> keypoints;
	sift_detector->detect(frame->getData(), keypoints);

	//filterKeypoints(keypoints);

	return keypoints;
}

void CalibrationProcessor::filterKeypoints(std::vector<cv::KeyPoint> kp) {
	cv::Point locations[4];

	if (frame->getID() % 10 == 0) {
		std::cout << historic_features.size() << "\n";
		historic_features.pop_back();

		/*
		for (int i = historic_features.size(); i > historic_features.size() - 4; i--) {
			locations[i] = historic_features[i].pt;
		}

		*/
	}

	else {
		for (cv::KeyPoint keypoint : kp) {
			if (historic_features.size() < 10) {
				if (!filterLocations(keypoint.pt))
					std::cout << keypoint.pt.x << " " << keypoint.pt.y << "\n";
				historic_features.push_back(keypoint);
			}
		}
	}
}

bool CalibrationProcessor::filterLocations(cv::Point pt) {
	for (CalibrationParameters i : cameras_tested) {
		if (i.found_features == true) {
			continue;
		}

		i.camera_id = frame->getCameraID();
		
		/*
		if (foundFeature(pt)) {

		}
		*/
	}

	return true;
}
