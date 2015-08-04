
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;

	sift_detector = std::make_shared<cv::SiftFeatureDetector>(
		0,		// nFeatures
		4,		// nOctaveLayers
		0.04,	// contrastThreshold
		10,		// edgeThreshold
		1.6		// sigma
	);

	// Convert Frame to RGB
	cvtColor(frame->getData(), frame->getData(), CV_BGR2RGB);

	// Morph Frame
	calibratePosition();
	//calibrateLens();

	return frame;
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

	cv::drawKeypoints(frame->getData(), keypoints, frame->getData());
}

/*
 *
 *	Helper Functions
 *
 */

std::vector<cv::KeyPoint> CalibrationProcessor::getKeypoints() {
	std::vector<cv::KeyPoint> keypoints;
	sift_detector->detect(frame->getData(), keypoints);

	filterKeypoints(keypoints);

	return historic_features;
}

void CalibrationProcessor::filterKeypoints(std::vector<cv::KeyPoint> kp) {
	if (frame->getID() % 10 == 0) {
		std::cout << historic_features.size() << "\n";
		historic_features.pop_back();
	}

	else {
		for (cv::KeyPoint keypoint : kp) {
			if (historic_features.size() < 10) {
				historic_features.push_back(keypoint);
			}
		}
	}
}
