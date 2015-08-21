
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;

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

	/*
		Initial frame
	*/
	if (frame->getID() < camera_parameters.background_id ) {
		camera_parameters.background_id = frame->getID();
		camera_parameters.background = current_frame.clone();
	}

	subtractBackground(camera_parameters, current_frame);
	camera_movement.average_point = updateAverageLocation(current_frame);
	determineDirection(camera_movement);

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";
	camera_movement.previous_point = camera_movement.average_point;
}

/*
 *
 *	Helper Functions
 *
 */

cv::Point CalibrationProcessor::updateAverageLocation(cv::Mat temp) {
	cv::Point average_point;
	int num = cv::countNonZero(temp);

	if (num > 0) {
		std::vector<cv::Point> white_locs;
		cv::findNonZero(temp, white_locs);

		for each (cv::Point white in white_locs) {
			average_point.x += white.x;
			average_point.y += white.y;
		}

		average_point.x /= num;
		average_point.y /= num;

		return average_point;
	}

	else {
		return cv::Point ();
	}
}

void CalibrationProcessor::determineDirection(Movement & movement) {
	auto delta_x = movement.average_point.x - movement.previous_point.x;
	auto delta_y = movement.average_point.y - movement.previous_point.y;

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

void CalibrationProcessor::subtractBackground(CalibrationParameters & parameters, cv::Mat & currentframe) {
	/*
		Background Subtraction via an init_frame and Thresholding
	*/
	cv::absdiff(parameters.background, currentframe, currentframe);
	cv::erode(currentframe, currentframe, cv::Mat(), cv::Point(0, 0), 2, 1);
	cv::dilate(currentframe, currentframe, cv::Mat(1, 1, CV_8UC1), cv::Point(0, 0), 2, 1, 1);
	cv::threshold(currentframe, currentframe, 25, 255, CV_THRESH_BINARY);
}