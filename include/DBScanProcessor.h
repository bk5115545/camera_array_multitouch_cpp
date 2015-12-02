
#pragma once

#include <cmath>
#include <vector>
#include <algorithm>

#include "Processor.h"

typedef std::vector<cv::Point> Cluster;

class DBScanProcessor : public Processor {

private:
	cv::Mat motion_locations;
	cv::Mat motion_mat;

	int n_motion;

	Cluster visited_points;
	Cluster clustered_points;

	// DBSCAN Parameters
	int minPoints = 3;	// Note should be at least >= 2
	float maxDist = 5.0f;

	float getDistanceBetween(cv::Point c_loc, cv::Point p_loc);
	bool findPointInList(Cluster list, cv::Point p);

	Cluster getRegion(cv::Point loc);
	Cluster expandCluster(cv::Point loc, Cluster loc_neighbors);

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};