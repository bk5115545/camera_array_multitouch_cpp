
#include "DBScanProcessor.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>     

/*
	DBScanProcessor generates a list of clusters (list) based on density of the movement
	points. This is important for object / user segmentation. ClusterProcessor currently 
	implements the DBSCAN algorithm for cluster determination.
*/

std::shared_ptr<Frame> DBScanProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	masks[Mask::MOTION] = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_mask"));
	masks[Mask::VISITED] = cv::Mat::zeros(masks[Mask::MOTION].size(), masks[Mask::MOTION].type());
	masks[Mask::CLUSTERED] = cv::Mat::zeros(masks[Mask::MOTION].size(), masks[Mask::MOTION].type());
	motion_bb = cv::Rect(cv::Point(0, 0), masks[Mask::MOTION].size());

	cv::Mat motion_locs = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_locs"));
	
	std::vector<Cluster> clusters;

	for (int i = 0; i < motion_locs.size().height; i++) {
		cv::Point cur_pt = motion_locs.at<cv::Point>(i);

		if (!getState(Mask::VISITED, cur_pt)) {
			setState(Mask::VISITED, cur_pt, true);
			
			Cluster cur_neigh = getRegion(cur_pt);

			if (cur_neigh.size() >= minPoints) {
				clusters.push_back(expandCluster(cur_pt, cur_neigh));
			}
		}
	}

	current_frame->addFeature("clusters", clusters);

	return current_frame;
}

Cluster DBScanProcessor::getRegion(cv::Point loc) {
	int min_x = loc.x - (maxDist / 2);
	int min_y = loc.y - (maxDist / 2);

	Cluster neighbors;

	for (int j = min_y; j < (min_y + maxDist); j++) {
		for (int i = min_x; i < (min_x + maxDist); i++) {
			cv::Point current_loc (i, j);
			
			if (motion_bb.contains(current_loc)) {
				if (getState(Mask::MOTION, current_loc)) {
					neighbors.push_back(current_loc);
				}
			}
		}
	}

	return neighbors;
}

Cluster DBScanProcessor::expandCluster(cv::Point loc, Cluster loc_neighbors) {
	Cluster current_cluster;
	
	Cluster temp = loc_neighbors;

	for (int i = 0; i < temp.size(); i++) {
		if (!getState(Mask::VISITED, temp[i])) {
			setState(Mask::VISITED, temp[i], true);
			
			Cluster cur_neighbors = getRegion(temp[i]);

			if (cur_neighbors.size() >= minPoints) {
				temp.insert(temp.end(), cur_neighbors.begin(), cur_neighbors.end());
			}
		}

		if (!getState(Mask::CLUSTERED, temp[i])) {
			setState(Mask::CLUSTERED, temp[i], true);
			current_cluster.push_back(temp[i]);
		}
	}

	return current_cluster;
}

bool DBScanProcessor::getState(Mask mask, cv::Point pt) {
	if (masks[mask].at<uchar>(pt) == State::ACTIVE)
		return true;
	return false;
}

void DBScanProcessor::setState(Mask mask, cv::Point pt, bool active) {
	if (active)
		masks[mask].at<uchar>(pt) = State::ACTIVE;
	else
		masks[mask].at<uchar>(pt) = State::INACTIVE;
}