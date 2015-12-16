
#include <opencv2/opencv.hpp>

#include "CameraDevice.h"
#include "Transformer.h"
#include "Frame.h"

#include "MotionProcessor.h"
#include "HistoricMotionProcessor.h"
#include "ContourProcessor.h"
#include "DBScanProcessor.h"
#include "ClusterColorizerProcessor.h"

int main(int argc, char * argv[]) {
	// We are using the second rabbits dataset.
	cv::VideoCapture image_seq (
		"D:\\Projects\\camera_array_multitouch_cpp\\resources\\Testsets\\FBMS\\rabbits02\\rabbits02_%4d.jpg"
	);
	
	cv::VideoWriter vid_output (
		"D:\\Projects\\camera_array_multitouch_cpp\\resources\\program-output\\output.avi", static_cast<int>(image_seq.get(CV_CAP_PROP_FOURCC)), 
		image_seq.get(CV_CAP_PROP_FPS), cv::Size((int) image_seq.get(CV_CAP_PROP_FRAME_WIDTH), (int) image_seq.get(CV_CAP_PROP_FRAME_HEIGHT)), true
	);

	Transformer main_chain;
	main_chain.addProcessor(new MotionProcessor());
	main_chain.addProcessor(new DBScanProcessor());
	main_chain.addProcessor(new ClusterColorizerProcessor());

	cv::Mat image;

	while (true) {
		image_seq >> image;

		if (image.empty())
			break;

		std::shared_ptr<Frame> frame = std::make_shared<Frame>(image, 0);
		main_chain.addFrame(frame);
		frame = main_chain.getResult();
		
		image = boost::any_cast<cv::Mat>(frame->getFeature("color_clusters"));

		cv::imshow("blah", image);
		cv::waitKey(5);
	}

	image_seq.release();
	vid_output.release();

	return 0;
}
