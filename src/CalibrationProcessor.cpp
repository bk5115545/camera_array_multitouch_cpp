
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;

	cv::Mat temp = frame->getData();
	cv::GaussianBlur(temp, temp, cv::Size(1, 1), 0.0, 0.0, cv::BORDER_DEFAULT);
	cvtColor(temp, temp, CV_BGR2GRAY);

	//calibrateLens(temp);
	calibratePosition(temp); 

	return std::make_shared<Frame>(temp, frame->getCameraID(), frame->getID());
}

/*
	calibrateLens
*/
void CalibrationProcessor::calibrateLens(cv::Mat & current_frame) {
	auto start = std::chrono::system_clock::now();

	// Surf ~500 ms

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";
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
	//cvCvtColor(current_frame, current_frame, CV_GRAY2RGB);
	
	cvtColor(current_frame, current_frame, CV_GRAY2RGB);
	cv::circle(current_frame, camera_movement.previous_point, 5, cv::Scalar(128, 128, 255), 5.0);
	cv::circle(current_frame, camera_movement.center_of_mass, 5, cv::Scalar(255, 128, 128), 5.0);

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


/*
	updateCenterOfMass estimates the center of an object by detecting the average location
	of all non-zero pixels in a binary mask.

	INPUT
		movement	- Used to set the center of mass variable
		temp		- Binary Mask which is the current frame
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

	else {
		movement.loc_history.clear();
	}
}

/*
	updateAverageLocation estimates the wall the center of mass is closest too. This is
	useful for knowing where the center of mass is traveling between cameras. 
	updateAverageLocation will average the loc_history and then will know what wall it is 
	closest to

	INPUT
		movement - Used for the loc_history and will set the location

	WARNING
		Not correctly implemented yet... I think
*/
void CalibrationProcessor::updateAverageLocation(PositionCalibration & movement) {
	
	if (movement.loc_history.size() > 0) {
		int sum = 0;

		for (auto wall : movement.loc_history) {
			sum += wall;
		}

		int average = int(floor(sum / movement.loc_history.size()));
		
		movement.last_known_position = average;

		std::cout << average << " " << movement.loc_history.size() << " " << sum << "\n";
	}
}

/*
	determineDirection will determine the direction the center of mass is moving inside
	a cameras view. This is useful for understanding internal movements. determineDirection
	does this by calculating the delta between the current position and previous position
	and comparing it against 0

	INPUT
		movement - will use center_of_mass and previous_points. will set left, right, up, down
*/
void CalibrationProcessor::determineDirection(PositionCalibration & movement) {
	auto delta_x = movement.center_of_mass.x - movement.previous_point.x;
	auto delta_y = movement.center_of_mass.y - movement.previous_point.y;

	// Left - Right
	if (delta_x < 0) {
		movement.left = true;
		movement.right = false;

		movement.loc_history.push_back(1);
	}

	else if (delta_x > 0) {
		movement.right = true;
		movement.left = false;

		movement.loc_history.push_back(0);
	}

	// Up - Down
	if (delta_y < 0) {
		movement.down = true;
		movement.up = false;

		movement.loc_history.push_back(3);
	}

	else if (delta_y > 0) {
		movement.up = true;
		movement.down = false;

		movement.loc_history.push_back(2);
	}
}

/*
	subtractBackground will modify current_frame to be a foreground mask. It does this by 
	using the first frame as the background and diffing it against the current frame. This 
	method is much faster and more accurate than the history based opencv implementation.

	INPUT
		parameters - Used to get the background
		current_frame - Will become foreground mask

	WARNING
		This implementation will change because it is based on the first frame.
*/
void CalibrationProcessor::subtractBackground(CalibrationParameters & parameters, cv::Mat & current_frame) {
	/*
		Background Subtraction via an init_frame and Thresholding
	*/
	cv::absdiff(parameters.background, current_frame, current_frame);
	cv::erode(current_frame, current_frame, cv::Mat(), cv::Point(0, 0), 2, 1);
	cv::dilate(current_frame, current_frame, cv::Mat(1, 1, CV_8UC1), cv::Point(0, 0), 2, 1, 1);
	cv::threshold(current_frame, current_frame, 25, 255, CV_THRESH_BINARY);
}

void CalibrationProcessor::extractFeatures(LensCalibration & lens, CalibrationParameters & parameters) {

}