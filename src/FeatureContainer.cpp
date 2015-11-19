
#include "FeatureContainer.h"

void FeatureContainer::set(std::string ID, Feature feature) {
	features[ID] = feature;
}

Feature FeatureContainer::get(std::string ID) {
	try {
		return features.at(ID);
	} catch (std::out_of_range e) {
		return NULL;
	}
}

bool FeatureContainer::check(std::string ID) {
	try {
		features.at(ID);
		return true;
	}
	catch (std::out_of_range e) {
		return false;
	}
}