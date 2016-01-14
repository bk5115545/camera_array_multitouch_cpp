
#pragma once

#include <vector>

#include "Processor.h"

typedef std::vector<cv::Point> Cluster;

class DBScanProcessor : public Processor {

private:
	/* Masks */
	cv::Mat masks [3];

	enum Mask {
		MOTION,
		VISITED,
		CLUSTERED
	};

	enum State {
		INACTIVE = 0,
		ACTIVE = 255
	};

	cv::Rect motion_bb;

	// DBSCAN Parameters
	int minPoints = 4;	// Note should be at least >= 2
	int maxDist = 10;	// Note should be at least >= 3

	Cluster getRegion(cv::Point loc);
	Cluster expandCluster(cv::Point loc, Cluster loc_neighbors);

	/* Mask Management */
	bool getState(Mask mask, cv::Point pt);
	void setState(Mask mask, cv::Point pt, bool active);

public:
	std::shared_ptr<Frame> computeFrame(std::shared_ptr<Frame> current_frame);
};