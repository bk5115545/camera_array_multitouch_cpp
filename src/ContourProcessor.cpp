#include "ContourProcessor.h"


std::shared_ptr<Frame> ContourProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	cv::Mat current_mat = current_frame->getData();

	cv::vector<cv::vector<cv::Point>> contours;


	if (first_frame) {
		first_frame = false;
		previous_mat = current_mat;
	}

	cv::Mat cleaned_up = cv::Mat(current_mat.size(), current_mat.type());

	//threshold
	cv::inRange(cleaned_up, cv::Scalar(0, 48, 80), cv::Scalar(20, 255, 255), cleaned_up);

	//clean it up
	cv::dilate(cleaned_up, cleaned_up, cv::Mat(), cv::Point(-1, -1), 2, 1);
	cv::GaussianBlur(cleaned_up, cleaned_up, cv::Point(5, 5), 14, 14);

	cv::erode(cleaned_up, cleaned_up, cv::Mat(), cv::Point(-1, -1), 3, 1);
	cv::dilate(cleaned_up, cleaned_up, cv::Mat(), cv::Point(-1, -1), 4, 1);
	cv::erode(cleaned_up, cleaned_up, cv::Mat(), cv::Point(-1, -1), 2, 1);

	previous_mat = current_mat;

	return std::make_shared<Frame>(Frame(cleaned_up, current_frame->getCameraID(), current_frame->getID()));


	//cv::imshow("cleanup", frame);
	/*
	//find edges
	cv::Mat edges;
	cv::Canny(frame,edges,40,100,3,true);
	//cv::imshow("edge detect", edges);

	cv::findContours(frame,contours,cv::RETR_LIST,cv::CHAIN_APPROX_TC89_L1,cv::Point(0,0));

	cv::Mat contourMat = cv::Mat::zeros(edges.size(),CV_8UC3);
	cv::Scalar color = cv::Scalar(255,255,255);
	cv::drawContours(contourMat,contours,-1,color,2,8);

	//cv::imshow("All Contours", contourMat);

	if(contours.size() > 0) {
		cv::vector<cv::vector<cv::Point>> interestingContours;

		for(int i = 1; i < contours.size(); i++) {
			//printf("area:\t%f\n", cv::contourArea(contours[i]));
			if(cv::contourArea(contours[i]) > 100) {
				interestingContours.push_back(contours[i]);
			}
		}

		cv::Mat interestingContour = cv::Mat::zeros(contourMat.size(),CV_8UC3);

		if(interestingContours.size() > 0) {
			cv::drawContours(interestingContour,interestingContours,-1,cv::Scalar(255,0,255),2,8);
		}

		return std::make_shared<Frame>(Frame(interestingContour, current_frame->getCameraID(), current_frame->getID()));
	}

	return current_frame;
	*/
}
