
#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include <map>

#include "Processor.h"
#include "DBPoint.h"

typedef std::vector<DBPoint> Cluster;

class DBScanProcessor : public Processor {

private:
	cv::Mat motion_locations;
	cv::Mat motion_mat;

	int n_motion;

	// DBSCAN Parameters
	int minPoints = 5;	// Note should be at least >= 2
	float maxDist = 25.0f;

	Cluster getRegion(DBPoint loc, Cluster & neighbors);
	Cluster expandCluster(DBPoint loc, Cluster loc_neighbors);

public:
	DBScanProcessor();

	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};