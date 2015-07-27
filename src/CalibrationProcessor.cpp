
#include <iostream>

#include "CalibrationProcessor.h"

std::shared_ptr<Frame> CalibrationProcessor::run(std::shared_ptr<Frame> frame) {
	std::cout << "Camera ID: " << frame->getCameraID() << "\n";
	return std::shared_ptr<Frame>(); //return nullptr
}

/*
	calibrateLens
*/
void CalibrationProcessor::calibrateLens() {

}


/*
	calibratePosition
*/
void CalibrationProcessor::calibratePosition() {

}