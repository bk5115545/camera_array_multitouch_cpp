#include "Transformer.h"

std::atomic<int> Transformer::class_threads = 0;

Transformer::Transformer(int thread_count) : jobs(1000), results(1000) {
	max_threads = thread_count;
}

Transformer::~Transformer() {

}

void Transformer::run() {

	while (job_count.fetch_add(-1) > 0) { //TODO check that this works correctly
		Frame* raw;
		jobs.pop(raw);

		cv::Mat frame = *raw;

		cv::vector<cv::vector<cv::Point>> contours;
		cvtColor(frame, frame, CV_BGR2HSV);
		cv::inRange(frame, cv::Scalar(0, 48, 80), cv::Scalar(20, 255, 255), frame);
		//cv::inRange(frame, cv::Scalar(0, 49, 38), cv::Scalar(19, 121, 255), frame);
		//cv::inRange(frame, cv::Scalar(cv::getTrackbarPos("H-Low", "Parameters"), cv::getTrackbarPos("S-Low", "Parameters"), cv::getTrackbarPos("V-Low", "Parameters")), 
		//				   cv::Scalar(cv::getTrackbarPos("H-High", "Parameters"), cv::getTrackbarPos("S-High", "Parameters"), cv::getTrackbarPos("V-High", "Parameters")), frame);
		//cv::imshow("range", frame);

		//clean it up
		cv::dilate(frame, frame, cv::Mat(), cv::Point(-1, -1), 2, 1);
		cv::GaussianBlur(frame, frame, cv::Point(5, 5), 14, 14);

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

			results.push(new Frame(&interestingContour, raw->getCameraID(), raw->getID()));
			//cv::imshow(std::string("Camera ") + std::to_string(raw->getCameraID()), interestingContour);
		}

	}
}

int Transformer::totalTransformerThreads() {
	return class_threads;
}

int Transformer::enqueue(Frame* frame) {
	jobs.push(frame);
	job_count++;

	if (instance_threads < job_count && instance_threads < max_threads) {
		threads.push_back(new std::thread(&Transformer::run, this));
		instance_threads++;
	}

	return job_count/(instance_threads + 1);
}

Frame* Transformer::popResult(bool blocking) {
	Frame* frame;
	results.pop(frame);
	return frame;
}


std::vector<Frame*> Transformer::stop_threads() {
	job_count = 0;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::vector<Frame*> unfinished_jobs;

	bool has_job = jobs.empty();

	while (has_job) {
		Frame* frame;
		jobs.unsynchronized_pop(frame);
		unfinished_jobs.push_back(frame);
	}

	return unfinished_jobs;
}