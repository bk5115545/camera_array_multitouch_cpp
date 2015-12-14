
#pragma once

#include <vector>

#include "Processor.h"

struct DBPoint {
	cv::Point pt;

	bool visited = false;
	bool clustered = false;
};

typedef std::vector<DBPoint> Cluster;

class DBScanProcessor : public Processor {

private:
	cv::Mat motion_locations;
	cv::Mat motion_mat;
	cv::Rect motion_bb;

	// DBSCAN Parameters
	int minPoints = 5;	// Note should be at least >= 2
	float maxDist = 25.0f;

	void getRegion(DBPoint loc, Cluster & neighbors);
	Cluster expandCluster(DBPoint loc, Cluster loc_neighbors);

public:
	DBScanProcessor();
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};