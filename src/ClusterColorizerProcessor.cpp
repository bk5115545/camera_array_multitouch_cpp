
#include "ClusterColorizerProcessor.h"

std::shared_ptr<Frame> ClusterColorizerProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	
	cv::Mat color_clusters = cv::Mat(current_frame->getData().size(), current_frame->getData().type());
	
	std::vector<std::vector<cv::Point>> clusters = boost::any_cast<std::vector<std::vector<cv::Point>>>(current_frame->getFeature("clusters"));

	std::array<cv::Vec3b, 10> colors = {
		cv::Vec3b (255, 0, 0),
		cv::Vec3b(0, 255, 0),
		cv::Vec3b(0, 0, 255),
		cv::Vec3b(128, 0, 0),
		cv::Vec3b(128, 128, 0),
		cv::Vec3b(0, 128, 0),
		cv::Vec3b(0, 128, 128),
		cv::Vec3b(0, 255, 255),
		cv::Vec3b(0, 0, 128),
		cv::Vec3b(128, 0, 128)
	};

	int current_color = 0;

	for (int i = 0; i < clusters.size(); i++) {
		for (int j = 0; j < clusters[i].size(); j++)
			color_clusters.at<cv::Vec3b>(clusters[i][j]) = colors[current_color % colors.size()];

		current_color++;
	}

	return std::make_shared<Frame>(color_clusters, current_frame->getCameraID(), current_frame->getID());
}
