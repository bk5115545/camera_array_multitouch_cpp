
#include "DBScanProcessor.h"

/*
	DBScanProcessor generates a list of clusters (list) based on density of the movement
	points. This is important for object / user segmentation. ClusterProcessor currently 
	implements the DBSCAN algorithm for cluster analysis which incorporates the concept of 
	k nearest neighbors
*/

DBScanProcessor::DBScanProcessor() {}

std::shared_ptr<Frame> DBScanProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	std::vector<Cluster> clusters;
	DBPoint current_point;
	Cluster current_neighbors (100);

	motion_mat = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_mask"));
	motion_locations = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_locs"));
	n_motion = motion_locations.rows;

	for (int i = 0; i < n_motion; i++) {
		current_point.pt = motion_locations.at<cv::Point>(i);

		if (current_point.visited)
			continue;

		current_point.visited = true;

		getRegion(current_point, current_neighbors);

		if (current_neighbors.size() >= minPoints)
			clusters.push_back(expandCluster(current_point, current_neighbors));
	}

	current_frame->addFeature("clusters", clusters);

	return current_frame;
}

Cluster DBScanProcessor::getRegion(DBPoint loc, Cluster & neighbors) {
	neighbors.push_back(loc);

	int min_x = loc.pt.x - (maxDist / 2);
	int min_y = loc.pt.y - (maxDist / 2);
	
	int max_x = loc.pt.x + (maxDist / 2);
	int max_y = loc.pt.y + (maxDist / 2);

	cv::Rect bounding_rect (cv::Point(), motion_mat.size());
	DBPoint current_point;

	for (int i = min_x; i < max_x; i++) {
		for (int j = min_y; j < max_y; j++) {
			current_point.pt = cv::Point(i, j);

			if (bounding_rect.contains(current_point.pt))
				if (motion_mat.at<uchar>(current_point.pt) == 255)
					neighbors.push_back(current_point);
		}
	}

	return neighbors;
}

Cluster DBScanProcessor::expandCluster(DBPoint loc, Cluster loc_neighbors) {
	DBPoint current_point = loc;
	Cluster current_cluster = loc_neighbors;
	Cluster current_neighbors (100);

	current_cluster.push_back(current_point);

	for (int i = 0; i < loc_neighbors.size(); i++) {
		 current_point = loc_neighbors[i];

		if (!current_point.visited) {
			current_point.visited = true;
			
			getRegion(current_point, current_neighbors);

			if (current_neighbors.size() >= minPoints)
				current_cluster.insert(current_cluster.end(), current_neighbors.begin(), current_neighbors.end());
		}

		if (!current_point.clustered) {
			current_point.clustered = true;
			current_cluster.push_back(current_point);
		}
	}

	return current_cluster;
}
