
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;

	//calibrateLens(frame->getData());

	cv::Mat temp = frame->getData();
	cv::GaussianBlur(temp, temp, cv::Size(1, 1), 0.0, 0.0, cv::BORDER_DEFAULT);
	cvtColor(temp, temp, CV_BGR2GRAY);

	calibratePosition(temp);

	return std::make_shared<Frame>(temp, frame->getCameraID(), frame->getID());
}

/*
	calibrateLens
*/
void CalibrationProcessor::calibrateLens(cv::Mat & current_frame) {
	std::shared_ptr<CameraDevice> camera = CameraDevice::devices[frame->getCameraID()];

	std::cout << camera->getOpenCVProperty(CV_CAP_PROP_CONTRAST) << "\n";
	camera->setOpenCVProperty(CV_CAP_PROP_CONTRAST, 30);
}

/*
	calibratePosition
*/
void CalibrationProcessor::calibratePosition(cv::Mat & current_frame) {
	auto start = std::chrono::system_clock::now();

	/* Get background frame */
	if (frame->getID() < camera_parameters.background_id ) {
		camera_parameters.background_id = frame->getID();
		camera_parameters.background = current_frame.clone();
	}

	subtractBackground(camera_parameters, current_frame);
	updateCenterOfMass(camera_movement, current_frame);
	determineDirection(camera_movement);
	//updateAverageLocation(camera_movement);

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";
	camera_movement.previous_point = camera_movement.center_of_mass;
}

/*
 *
 *	Helper Functions
 *
 */

void CalibrationProcessor::updateCenterOfMass(PositionCalibration & movement, cv::Mat & temp) {
	int num = cv::countNonZero(temp);

	if (num > 0) {
		std::vector<cv::Point> white_locs;
		cv::findNonZero(temp, white_locs);

		for each (cv::Point white in white_locs) {
			movement.center_of_mass.x += white.x;
			movement.center_of_mass.y += white.y;
		}

		movement.center_of_mass.x /= num;
		movement.center_of_mass.y /= num;
	}
}

void CalibrationProcessor::updateAverageLocation(PositionCalibration & movement) {
	int sum_x = 0; 
	int sum_y = 0;

	for each (cv::Point location in movement.loc_history) {
		sum_x += location.x;
		sum_y += location.y;
	}

	int average_x = floor(sum_x / movement.loc_history.size());
	int average_y = floor(sum_y / movement.loc_history.size());

	std::cout << average_x << " " << average_y << "\n";
}

void CalibrationProcessor::determineDirection(PositionCalibration & movement) {
	auto delta_x = movement.center_of_mass.x - movement.previous_point.x;
	auto delta_y = movement.center_of_mass.y - movement.previous_point.y;

	// Left - Right
	if (delta_x < 0) {
		movement.left = true;
		movement.right = false;
	}

	else if (delta_x > 0) {
		movement.right = true;
		movement.left = false;
	}

	// Up - Down
	if (delta_y < 0) {
		movement.down = true;
		movement.up = false;
	}

	else if (delta_y > 0) {
		movement.up = true;
		movement.down = false;
	}
}

void CalibrationProcessor::subtractBackground(CalibrationParameters & parameters, cv::Mat & current_frame) {
	/*
		Background Subtraction via an init_frame and Thresholding
	*/
	cv::absdiff(parameters.background, current_frame, current_frame);
	cv::erode(current_frame, current_frame, cv::Mat(), cv::Point(0, 0), 2, 1);
	cv::dilate(current_frame, current_frame, cv::Mat(1, 1, CV_8UC1), cv::Point(0, 0), 2, 1, 1);
	cv::threshold(current_frame, current_frame, 25, 255, CV_THRESH_BINARY);
}