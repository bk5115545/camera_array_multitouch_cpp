
#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <chrono>

#include "FeatureContainer.h"

class Frame : public std::enable_shared_from_this<Frame> {
	private:
		cv::Mat _frame;
		int _camera_id;
		unsigned long long _frame_id;

		FeatureContainer features;

	public:
		Frame();
		Frame(cv::Mat source, int camera_id);
		Frame(cv::Mat source, int camera_id, unsigned long long frame_id);
		
		cv::Mat getData();
		int getCameraID();
		unsigned long long getID();

		void addFeature(std::string ID, Feature feature);
		Feature getFeature(std::string ID);
		bool checkFeature(std::string ID);

		operator cv::Mat() const;

		bool operator == (Frame f1);
};