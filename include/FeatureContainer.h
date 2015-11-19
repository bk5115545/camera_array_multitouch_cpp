#pragma once

#include <map>
#include <memory>
#include <string>

#include <boost/any.hpp>

// use boost::any_cast to cast the feature
typedef boost::any Feature;

class FeatureContainer {

private:
	std::map<std::string, Feature> features;

public:
	void set(std::string ID, Feature feature);
	Feature get(std::string ID);
	bool check(std::string ID);
};

