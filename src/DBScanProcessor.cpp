
#include "DBScanProcessor.h"

#include <iostream>

/*
	DBScanProcessor generates a list of clusters (list) based on density of the movement
	points. This is important for object / user segmentation. ClusterProcessor currently 
	implements the DBSCAN algorithm for cluster determination.
*/

DBScanProcessor::DBScanProcessor() {
	motion_bb = cv::Rect(0, 0, motion_mat.cols, motion_mat.rows);
}

std::shared_ptr<Frame> DBScanProcessor::computeFrame(std::shared_ptr<Frame> current_frame) {
	DBPoint current_point;
	std::vector<Cluster> clusters;
	
	motion_mat = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_mask"));
	motion_locations = boost::any_cast<cv::Mat>(current_frame->getFeature("motion_locs"));

	for (int i = 0; i < motion_locations.rows; i++) {
		current_point.pt = motion_locations.at<cv::Point>(i);

		if (!current_point.visited) {
			current_point.visited = true;

			Cluster current_neighbors (100);
			getRegion(current_point, current_neighbors);

			if (current_neighbors.size() >= minPoints)
				clusters.push_back(expandCluster(current_point, current_neighbors));
		}
	}
	
	current_frame->addFeature("clusters", clusters);
	return current_frame;
}

void DBScanProcessor::getRegion(DBPoint loc, Cluster & neighbors) {
	int min_x = loc.pt.x - (maxDist / 2);
	int min_y = loc.pt.y - (maxDist / 2);

	cv::Rect bounding_rect(cv::Point(), motion_mat.size());
	DBPoint current_point;

	neighbors.push_back(loc);
	
	cv::Rect bounding_search_box(cv::Point(min_x, min_y), cv::Point(min_x + maxDist, min_y + maxDist));

	for (int i = 0; i < motion_locations.rows; i++) {
		current_point.pt = motion_locations.at<cv::Point>(i);

		if (bounding_search_box.contains(current_point.pt))
			neighbors.push_back(current_point);
	}

	/*
	for (int i = min_x; i < (min_x + maxDist); i++) {
		for (int j = min_y; j < (min_y + maxDist); j++) {
			current_point.pt = cv::Point(loc.pt.x + i, loc.pt.y + j);

			if (bounding_rect.contains(current_point.pt))
				if (motion_mat.at<uchar>(current_point.pt) == 255)
					neighbors.push_back(current_point);
		}
	}*/

	/*
	cv::Mat region (cv::Size(maxDist, maxDist), motion_mat.type());
	
	cv::Mat nonZeroLocs;
	cv::Point roi_loc;
	cv::Point transformed;

	float midPt = maxDist / 2;

	cv::Mat temp (motion_mat.size(), motion_mat.type());;

	cv::getRectSubPix(motion_mat, region.size(), loc.pt, region);
	cv::findNonZero(region, nonZeroLocs);

	for (int i = 0; i < nonZeroLocs.rows; i++) {
		cv::Point r_loc = nonZeroLocs.at<cv::Point>(i);
		cv::Point a (loc.pt.x + (midPt - r_loc.x), loc.pt.y + (midPt - r_loc.y));

		temp.at<cv::Vec3b>(r_loc) = cv::Vec3b(0, 0, 0);
	}

	return temp;
	*/
	/*
	for (int i = 0; i < region.rows; i++) {
		for (int j = 0; j < region.cols; j++) {
			cv::Vec3b color = region.at<cv::Vec3b>(cv::Point(j, i));

			std::cout << (int)color[0] << (int)color[1] << (int)color[2] << std::endl;
		}
	}
	*/
	//cv::findNonZero(region, nonZeroLocs);

/*
	neighbors.push_back(loc);
	for (int i = 0; i < nonZeroLocs.rows; i++) {
		roi_loc = nonZeroLocs.at<cv::Point>(i);

		// Transform roi coordinate space to motion_mat cordinate space
		transformed = cv::Point(loc.pt.x + (midpt - roi_loc.x), loc.pt.y + (midpt - roi_loc.y));
		
		if (motion_bb.contains(transformed))
			neighbors.push_back(DBPoint(transformed));
	}
*/
}

Cluster DBScanProcessor::expandCluster(DBPoint loc, Cluster loc_neighbors) {
	DBPoint current_point = loc;
	Cluster current_cluster = loc_neighbors;

	current_cluster.push_back(current_point);

	for (int i = 0; i < loc_neighbors.size(); i++) {
		current_point = loc_neighbors[i];

		if (!current_point.visited) {
			current_point.visited = true;
			
			Cluster current_neighbors (100);
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
