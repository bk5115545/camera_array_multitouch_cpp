
#include "ClusterColorizerProcessor.h"

ClusterColorizerProcessor::ClusterColorizerProcessor() {
	colors[0] = cv::Vec3b(255, 0, 0);
	colors[1] = cv::Vec3b(0, 255, 0);
	colors[2] = cv::Vec3b(0, 0, 255);
	colors[3] = cv::Vec3b(128, 0, 0);
	colors[4] = cv::Vec3b(128, 128, 0);
	colors[5] = cv::Vec3b(0, 128, 0);
	colors[6] = cv::Vec3b(0, 128, 128);
	colors[7] = cv::Vec3b(0, 255, 255);
	colors[8] = cv::Vec3b(0, 0, 128);
	colors[9] = cv::Vec3b(128, 0, 128);
}

std::shared_ptr<Frame> ClusterColorizerProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	cv::Mat color_clusters = cv::Mat(current_frame->getData().size(), current_frame->getData().type());
	std::vector<Cluster> clusters = boost::any_cast<std::vector<Cluster>>(current_frame->getFeature("clusters"));

	int current_color = 0;

	for (int i = 0; i < clusters.size(); i++) {
		for (int j = 0; j < clusters[i].size(); j++)
			color_clusters.at<cv::Vec3b>(clusters[i][j].pt) = colors[current_color % colors.size()];

		current_color++;
	}

	current_frame->addFeature("color_clusters", color_clusters);

	return current_frame;
}
