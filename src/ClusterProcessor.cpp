
#include "ClusterProcessor.h"

/*
	ClusterProcessor generates a list of clusters (list) based on density of the movement
	points. This is important for object / user segmentation. ClusterProcessor currently 
	implements the DBSCAN algorithm for cluster analysis which incorporates the concept of 
	k nearest neighbors
*/

std::shared_ptr<Frame> ClusterProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	std::vector<Cluster> clusters;

	motion_mat = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_mask"));
	motion_locations = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_locs"));
	n_motion = motion_locations.rows;

	for (int i = 0; i < n_motion; i++) {
		cv::Point current_point = motion_locations.at<cv::Point>(i);

		if (findPointInList(visited_points, current_point))
			continue;

		visited_points.push_back(current_point);

		Cluster neighbors = getRegion(current_point);

		if (neighbors.size() < minPoints)
			noise.push_back(current_point);
		else {
			clusters.push_back(expandCluster(current_point, neighbors));
		}
	}

	current_frame->addFeature("clusters", clusters);
	
	return current_frame;
}

float ClusterProcessor::getDistanceBetween(cv::Point c_loc, cv::Point p_loc) {
	return sqrt(pow(abs(p_loc.x - c_loc.x), 2) + pow(abs(p_loc.y - c_loc.y), 2));
}

bool ClusterProcessor::findPointInList(Cluster list, cv::Point p) {
	auto iter = std::find_if(list.begin(), list.end(),
		[p] (cv::Point p2) -> bool {
			if ((p.x == p2.x) & (p.y == p2.y))
				return true;
			return false;
		});

	if (iter != list.end())
		return true;
	return false;
}

/*
	dist = side length of square
*/
Cluster ClusterProcessor::getRegion(cv::Point loc) {
	Cluster neighbors; 

	neighbors.push_back(loc);

	float min_x = loc.x - (maxDist / 2);
	float min_y = loc.y - (maxDist / 2);
	
	float max_x = loc.x + (maxDist / 2);
	float max_y = loc.y + (maxDist / 2);

	cv::Rect bounding_rect (cv::Point(), motion_mat.size());

	for (int i = min_x; i < max_x; i++) {
		for (int j = min_y; j < max_y; j++) {
			cv::Point current_point (i, j);
	
			if (bounding_rect.contains(current_point))
				if (motion_mat.at<uchar>(current_point) == 255)
					neighbors.push_back(current_point);
		}
	}

	return neighbors;
}

Cluster ClusterProcessor::expandCluster(cv::Point loc, Cluster loc_neighbors) {
	Cluster current = loc_neighbors;

	current.push_back(loc);

	for (int i = 0; i < loc_neighbors.size(); i++) {
		cv::Point p = loc_neighbors[i];

		if (!findPointInList(visited_points, p)) {
			visited_points.push_back(p);
			
			Cluster p_neighbors = getRegion(p);

			if (p_neighbors.size() >= minPoints) {
				// append the cluster to the current
				current.insert(current.end(), p_neighbors.begin(), p_neighbors.end());
			}
		}

		if (!findPointInList(clustered_points, p)) {
			clustered_points.push_back(p);
			current.push_back(p);
		}
	
	}

	return current;
}