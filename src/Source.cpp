
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "MotionProcessor.h"
#include "HistoricMotionProcessor.h"
#include "ContourProcessor.h"
#include "DBScanProcessor.h"
#include "ClusterColorizerProcessor.h"

int main(int argv, char ** argc) {
	bool rendering = true;

	std::vector<std::shared_ptr<CameraDevice>> devices = std::vector<std::shared_ptr<CameraDevice>>();

	for(int i = 0; i < 8; i++) {
		std::shared_ptr<CameraDevice> dev = std::make_shared<CameraDevice>(CameraDevice(i));
		
		if(dev->acquire()) {
			devices.push_back(dev);
			cv::namedWindow(dev->getName());
		}
	}

	Transformer main_chain;

	main_chain.addProcessor(new MotionProcessor());
	main_chain.addProcessor(new DBScanProcessor());
	main_chain.addProcessor(new ClusterColorizerProcessor());

	while (rendering) {
		for (std::shared_ptr<CameraDevice> dev : devices) {
			std::shared_ptr<Frame> frame = dev->getFrame();
			
			main_chain.addFrame(frame);
			frame = main_chain.getResult();
			
			cv::imshow("motion mask", boost::any_cast<cv::Mat>(frame->getFeature("motion_mask")));
			cv::imshow(dev->getName(), boost::any_cast<cv::Mat>(frame->getFeature("color_clusters")));
		}

		if (cv::waitKey(2) >= 0)
			rendering = false;
	}

	main_chain.stopProcessors();

	return 0;
}
