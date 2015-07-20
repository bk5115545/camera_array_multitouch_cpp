#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <map>

#include "FeatureContainer.h"

class Frame : public std::enable_shared_from_this<Frame> {

	private:
		cv::Mat _frame;
		int _camera_id;
		unsigned long long _frame_id;


		FeatureContainer theConatiner;

	public:
		Frame();
		Frame(cv::Mat source, int camera_id);
		Frame(cv::Mat source,int camera_id,unsigned long long frame_id);
		

		cv::Mat getData();
		int getCameraID();
		unsigned long long getID();

		operator cv::Mat() const;

};