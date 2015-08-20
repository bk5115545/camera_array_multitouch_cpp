
#include <iostream>
#include <chrono>
#include <algorithm>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> f) {
	frame = f;

	cv::Mat temp = calibratePosition();

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
cv::Mat CalibrationProcessor::calibratePosition() {
	auto start = std::chrono::system_clock::now();

	/*
		Set Up Temporary Mat Container
	*/
	cv::Mat temp = frame->getData();
	cv::GaussianBlur(temp, temp, cv::Size(1, 1), 0.0, 0.0, cv::BORDER_DEFAULT);
	cvtColor(temp, temp, CV_BGR2GRAY);

	/*
		Initial frame
	*/
	if (frame->getID() < first_frame_id ) {
		first_frame_id = frame->getID();
		first_frame = temp.clone();
	}

	/*
		Background Subtraction via Running Average and Thresholding
	*/
	cv::absdiff(first_frame, temp, temp);
	cv::erode(temp, temp, cv::Mat(), cv::Point(0, 0), 2, 1);
	cv::dilate(temp, temp, cv::Mat(1, 1, CV_8UC1), cv::Point(0, 0), 2, 1, 1);
	cv::threshold(temp, temp, 25, 255, CV_THRESH_BINARY);

	cv::Point average = updateAverageLocation(temp);
	cvtColor(temp, temp, CV_GRAY2RGB);
	cv::circle(temp, average, 5, cv::Scalar(255, 128, 128), 5.0);

	//std::cout << "X: " << average.x << " Y: " << average.y << "\n";

	//determineDirection();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now() - start).count();

	std::cout << duration << "\n";
	previous_point = average;

	return temp;
}

/*
 *
 *	Helper Functions
 *
 */

cv::Point CalibrationProcessor::updateAverageLocation(cv::Mat temp) {
	cv::Point average_point;
	int area = temp.rows * temp.cols;
	
	int num_white = 1;

	for (int y = 0; y < temp.rows; y++) {
		for (int x = 0; x < temp.cols; x++) {
			uchar color = temp.at<uchar>(y, x);

			if (color == 255) {
				average_point.x += x;
				average_point.y += y;

				num_white++;
			}
		}
	}	
	
	average_point.x /= num_white;
	average_point.y /= num_white;
	return average_point;
}

/*
void CalibrationProcessor::determineDirection() {
	auto delta_x = average_point.x - previous_point.x;
	auto delta_y = average_point.y - previous_point.y;

	if (delta_x < 0) {
		std::cout << "Moving Left" << "\n";
		number_left++;
	}

	else if (delta_x > 0) {
		std::cout << "Moving Right" << "\n";
		number_right++;
	}

	/*
	if (delta_y < 0) {
		std::cout << "Moving Down" << "\n";
	}

	else if (delta_y > 0) {
		std::cout << "Moving Up" << "\n";
	}
	
}

*/