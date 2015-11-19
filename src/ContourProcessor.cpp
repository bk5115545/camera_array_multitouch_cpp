#include "ContourProcessor.h"


std::shared_ptr<Frame> ContourProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	cv::Mat frame = current_frame->getData();

	cv::vector<cv::vector<cv::Point>> contours;
	cvtColor(frame,frame,CV_BGR2HSV);

	//threshold
	cv::inRange(frame,cv::Scalar(0,48,80),cv::Scalar(20,255,255),frame);
	
	//clean it up
	cv::dilate(frame,frame,cv::Mat(),cv::Point(-1,-1),2,1);
	cv::GaussianBlur(frame,frame,cv::Point(5,5),14,14);

	cv::erode(frame,frame,cv::Mat(),cv::Point(-1,-1),3,1);
	cv::dilate(frame,frame,cv::Mat(),cv::Point(-1,-1),4,1);
	cv::erode(frame,frame,cv::Mat(),cv::Point(-1,-1),2,1);
	//cv::imshow("cleanup", frame);

	//find edges
	cv::Mat edges;
	cv::Canny(frame,edges,40,100,3,true);
	//cv::imshow("edge detect", edges);

	cv::RNG rng = cv::RNG(54121315467);
	cv::findContours(frame,contours,cv::RETR_LIST,cv::CHAIN_APPROX_TC89_L1,cv::Point(0,0));

	cv::Mat contourMat = cv::Mat::zeros(edges.size(),CV_8UC3);
	cv::Scalar color = cv::Scalar(255,255,255);
	cv::drawContours(contourMat,contours,-1,color,2,8);


	//cv::imshow("All Contours", contourMat);

	if(contours.size() > 0) {
		cv::vector<cv::vector<cv::Point>> interestingContours;

		for(unsigned int i = 1; i < contours.size(); i++) {
			//printf("area:\t%f\n", cv::contourArea(contours[i]));
			if(cv::contourArea(contours[i]) > 10000) {
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
}

void ContourProcessor::setTrackingID(int id,cv::Point hsl_low,cv::Point hsl_high) {
	trackedParameters[id] = std::pair<cv::Point,cv::Point>(hsl_low,hsl_high);
}