#pragma once
#include <map>
#include <memory>
#include <string>

class FeatureContainer {

private:
	std::map<std::string, std::shared_ptr<void>> trackedFeatures;

public:
	FeatureContainer() {}

	void set(std::string ID,std::shared_ptr<void> feature) {
		trackedFeatures[ID] = feature;
	}

	std::shared_ptr<void> get(std::string ID) {
		try {
			return trackedFeatures.at(ID);
		}
		catch(std::out_of_range e) { return NULL; }
	}
};

typedef std::shared_ptr<void> Feature;