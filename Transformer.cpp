#include "Transformer.h"

std::atomic<int> Transformer::class_threads = 0;

Transformer::Transformer(int thread_count) {
	max_threads = thread_count;
	instance_threads = 0;
	job_count = 0;

	jobs = new boost::lockfree::queue<Frame*>(1000);
	results = new boost::lockfree::queue<Frame*>(1000);
	threads = std::vector<std::thread*>();
}

Transformer::~Transformer() {
	delete jobs;
	delete results;
}

void Transformer::run() {

	while (job_count.fetch_add(-1) > 0) { //TODO check that this works correctly
		Frame* raw;
		if (!jobs->pop(raw)) {
			
			std::cout << "Error no job for thread." << std::endl;
			break;
		}

		cv::Mat frame = raw->getData();
		
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

			// ::TODO:: Trace for memory leak
			results->push(new Frame(&interestingContour, raw->getCameraID(), raw->getID()));
			//cv::imshow(std::string("Camera ") + std::to_string(raw->getCameraID()), interestingContour);
		}

	}
	class_threads--;
	instance_threads--;
}

int Transformer::totalTransformerThreads() {
	return class_threads;
}

int Transformer::enqueue(Frame* frame) {
	// ::TODO:: Trace for memory leak
	//jobs->push(new Frame(&frame->getData(), frame->getCameraID(), frame->getID()));
	while (!jobs->push(frame))
		;

	job_count++;

	if (instance_threads < job_count && instance_threads < max_threads) {
		std::thread* newThread = new std::thread([=] { run(); });
		threads.push_back(newThread);
		//std::thread([=] { run(); });
		instance_threads++;
		class_threads++;
	}

	return 0;//job_count/(instance_threads + 1);
}

bool Transformer::popResult(Frame*& into, bool blocking) {
	return results->pop(into);
}


std::vector<Frame*> Transformer::stop_threads() {
	job_count = -6969; //the threads will close when the job count is 0
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::vector<Frame*> unfinished_jobs;

	bool has_job = jobs->empty();

	while (has_job) {
		Frame* frame;
		jobs->unsynchronized_pop(frame);
		unfinished_jobs.push_back(frame);
	}

	return unfinished_jobs;
}