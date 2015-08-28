#include <thread>
#include <memory>
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "MotionProcessor.h"
#include "BlobProcessor.h"

int main(int argv, char** argc) {
	bool rendering = true;
	std::vector<std::shared_ptr<CameraDevice>> devices = std::vector<std::shared_ptr<CameraDevice>>();

	for(int i=0; i<8; i++) {
		std::shared_ptr<CameraDevice> dev = std::make_shared<CameraDevice>(CameraDevice(i));
		
		if(dev->acquire()) {
			devices.push_back(dev);
			cv::namedWindow(dev->getName());
		}
	}

	Transformer<MotionProcessor> bp (1);
	Transformer<BlobProcessor> mp (1);

	while (rendering) {
		for(std::shared_ptr<CameraDevice> dev : devices) {
			std::shared_ptr<Frame> inputFrame = dev->getFrame();
			
			bp.enqueue(inputFrame);
			std::shared_ptr<Frame> result = bp.popResult();

			if(result.get() != nullptr) {
				cv::imshow(dev->getName(), result->getData());
				
				if(cv::waitKey(30) >= 0) 
					rendering = false;
			}
		}
	}
	
	return 0;
}
