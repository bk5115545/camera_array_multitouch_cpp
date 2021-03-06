
#include <opencv2/opencv.hpp>

#include <chrono>
#include <iostream>
#include <fstream>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "MotionProcessor.h"
#include "HistoricMotionProcessor.h"
#include "ContourProcessor.h"
#include "DBScanProcessor.h"
#include "ClusterColorizerProcessor.h"

int main(int argc, char * argv[]) {

	cv::VideoCapture image_seq (
		"D:\\Projects\\camera_array_multitouch_cpp\\resources\\Testsets\\FBMS\\rabbits03\\rabbits03_0%3d.jpg"
	);

	std::ofstream log;
	log.open("D:\\Projects\\camera_array_multitouch_cpp\\resources\\program-output\\log.txt");
	
	Transformer main_chain;
	
	main_chain.addProcessor(new MotionProcessor());
	main_chain.addProcessor(new DBScanProcessor());
	main_chain.addProcessor(new ClusterColorizerProcessor());
	
	cv::Mat image;

	std::vector<std::chrono::milliseconds> frame_times;

	auto total_start_time = std::chrono::steady_clock::now();
	int image_id = 0;
	while (true) {
		auto frame_start_time = std::chrono::steady_clock::now();

		image_seq >> image;

		if (image.empty())
			break;

		std::shared_ptr<Frame> frame = std::make_shared<Frame>(image, 0);
		main_chain.addFrame(frame);
		frame = main_chain.getResult();
		
		image = boost::any_cast<cv::Mat>(frame->getFeature("color_clusters"));
		
		auto frame_end_time = std::chrono::steady_clock::now();
		
		cv::imshow("Realtime Output", image);
		cv::imwrite("D:\\Projects\\camera_array_multitouch_cpp\\resources\\program-output\\output_" + std::to_string(image_id) + ".png", image);
		image_id++;
		cv::waitKey(5);

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (frame_end_time - frame_start_time);
		frame_times.push_back(duration);

		log << "Frame Processed in " << duration.count() << " ms" << std::endl;
	}

	auto total_end_time = std::chrono::steady_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::seconds> (total_end_time - total_start_time);
	log << "All Frames Processed in " << duration.count() << " s" << std::endl;

	double average = 0.0;

	for (auto i : frame_times) {
		average += i.count();
	}

	average /= frame_times.size();

	log << "Average Frame time " << average << " ms" << std::endl;

	log.close();

	image_seq.release();

	return 0;
}
