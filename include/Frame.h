#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>

class Frame {

	private:
		cv::Mat _frame;
		int _camera_id;
		int _frame_id;
		std::unordered_map<std::string, void*> features;

	public:
		Frame();
		Frame(cv::Mat* source, int camera_id, int frame_id);

		cv::Mat getData();
		int getCameraID();
		int getID();
		template<class T> T getProperty(std::string name, T t);

		operator cv::Mat() const;
};