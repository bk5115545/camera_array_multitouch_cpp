#include <thread>
#include <memory>
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "MotionProcessor.h"
#include "BlobProcessor.h"
#include "ContourProcessor.h"

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

	Transformer main_chain;

	// main_chain.addProcessor(new ContourProcessor);
	main_chain.addProcessor(new MotionProcessor());
	main_chain.addProcessor(new BlobProcessor());
	main_chain.addProcessor(new MotionProcessor());

	while (rendering) {
		auto start = std::chrono::system_clock::now();

		for (std::shared_ptr<CameraDevice> dev : devices) {
			std::shared_ptr<Frame> inputFrame = dev->getFrame();

			main_chain.addFrame(inputFrame);
			main_chain.processFrames();
			std::shared_ptr<Frame> result = main_chain.getResult();

			if (result.get() != nullptr) {
				cv::imshow(dev->getName(), result->getData());

				if (cv::waitKey(30) >= 0)
					rendering = false;
			}
		}

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (
			std::chrono::system_clock::now() - start).count();

		std::cout << duration << "\n";
	}

	return 0;
}
