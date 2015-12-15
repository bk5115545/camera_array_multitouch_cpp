
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
	cv::VideoCapture image_seq ("D:\\Projects\\camera_array_multitouch_cpp\\resources\\Testsets\\FBMS\\cars10\\cars10_%2d.jpg");
	
	cv::VideoWriter vid_output (
		"D:\\Projects\\camera_array_multitouch_cpp\\resources\\program-output\\output.avi", static_cast<int>(image_seq.get(CV_CAP_PROP_FOURCC)), 
		image_seq.get(CV_CAP_PROP_FPS), cv::Size((int) image_seq.get(CV_CAP_PROP_FRAME_WIDTH), (int) image_seq.get(CV_CAP_PROP_FRAME_HEIGHT)), true
	);

	cv::Mat image;

	while (true) {
		image_seq >> image;

		if (image.empty())
			break;

		vid_output << image;
	}

	std::cout << "Done" << std::endl;

	image_seq.release();
	vid_output.release();

	return 0;
}
