
#pragma once

#include <string>
#include <memory>
#include <chrono>

#include <boost/any.hpp>
#include <opencv2/opencv.hpp>

// use boost::any_cast to cast the feature
typedef boost::any Feature;

class Frame : public std::enable_shared_from_this<Frame> {
	private:
		cv::Mat _frame;
		int _camera_id;
		unsigned long long _frame_id;

		std::map<std::string, Feature> features;

	public:
		Frame();
		Frame(cv::Mat source, int camera_id);
		Frame(cv::Mat source, int camera_id, unsigned long long frame_id);
		
		cv::Mat getData();
		int getCameraID();
		unsigned long long getID();

		void addFeature(std::string ID, Feature feature);
		Feature getFeature(std::string ID);
		void removeFeature(std::string ID);
		bool checkFeature(std::string ID);

		bool operator == (Frame f1);
};