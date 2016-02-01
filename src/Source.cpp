#include <thread>
#include <memory>
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "BlobProcessor.h"
#include "ContourProcessor.h"

//#include "vld.h"

int main(int argv,char** argc) {
	bool rendering = true;
	std::vector<std::shared_ptr<CameraDevice>> devices = std::vector<std::shared_ptr<CameraDevice>>();

	for(int i=1; i<8; i++) {
		//printf("testing id %i\n",i);
		std::shared_ptr<CameraDevice> dev = std::make_shared<CameraDevice>(CameraDevice(i));
		if(dev->acquire()) {
			devices.push_back(dev);
			//printf("got id %i\n",i);
		}
	}

	Transformer<BlobProcessor> tr(4);

	for(std::shared_ptr<CameraDevice> dev : devices) {
		cv::namedWindow("testing " + std::to_string(dev->getID()),1);
	}

	while (rendering) {
		for(std::shared_ptr<CameraDevice> dev : devices) {
			dev->grabFrame();
		}

		for(std::shared_ptr<CameraDevice> dev : devices) {
			std::shared_ptr<Frame> inputFrame = dev->decodeFrame();

			tr.enqueue(inputFrame);
			//std::cout << "FrameID:" << inputFrame->getID() << "\tJob Length:" << res << std::endl;

			std::shared_ptr<Frame> result = tr.popResult();

			if(result.get() != nullptr) {
				cv::imshow("testing " + std::to_string(dev->getID()),result->getData());
				if(cv::waitKey(10) >= 0) rendering = false;
			}
		}
	}
	
	return 0;
}
