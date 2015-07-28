#include <thread>
#include <memory>
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "BlobProcessor.h"
#include "ContourProcessor.h"
#include "CalibrationProcessor.h"

//#include "vld.h"

int main(char* argsv,char argc) {
	bool rendering = true;
	CameraDevice::devices = std::vector<std::shared_ptr<CameraDevice>>();

	for(int i=0; i<8; i++) {
		printf("testing id %i\n",i);
		std::shared_ptr<CameraDevice> dev = std::make_shared<CameraDevice>(CameraDevice(i));

		if(dev->acquire()) {
			CameraDevice::devices.push_back(dev);
			printf("got id %i\n",i);
		}
	}

	Transformer<CalibrationProcessor> tr(2);
	//Transformer<CalibrationProcessor> cp(

	for (std::shared_ptr<CameraDevice> dev : CameraDevice::devices) {
		cv::namedWindow("testing " + std::to_string(dev->getID()), 1);
	}

	while (rendering) {
		for (std::shared_ptr<CameraDevice> dev : CameraDevice::devices) {
			dev->grabFrame();
		}

		for (std::shared_ptr<CameraDevice> dev : CameraDevice::devices) {
			std::shared_ptr<Frame> inputFrame = dev->decodeFrame();

			tr.enqueue(inputFrame);
			//std::cout << "FrameID:" << inputFrame->getID() << "\tJob Length:" << res << std::endl;

			std::shared_ptr<Frame> result = tr.popResult();

			if(result.get() != nullptr) {
				cv::imshow("testing " + std::to_string(dev->getID()),result->getData());
				if(cv::waitKey(10) >= 0) 
					rendering = false;
			}
		}
	}
	
	return 0;
}
