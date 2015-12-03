
#include "DBPoint.h"

DBPoint::DBPoint() {
	visited = false;
	clustered = false;
}

DBPoint::DBPoint(cv::Point pt) {
	this->pt = pt;

	visited = false;
	clustered = false;
}