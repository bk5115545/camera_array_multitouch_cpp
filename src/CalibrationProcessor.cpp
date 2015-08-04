
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

	filterKeypoints(keypoints);

	return historic_features;
}

void CalibrationProcessor::filterKeypoints(std::vector<cv::KeyPoint> kp) {
	cv::Point locations[4];

	if (frame->getID() % 10 == 0) {
		std::cout << historic_features.size() << "\n";
		historic_features.pop_back();

		for (int i = historic_features.size(); i > historic_features.size() - 4; i--) {
			locations[i] = historic_features[i].pt;
		}
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
		
		if (foundFeature(pt)) {

		}

	}

	return true;
}
