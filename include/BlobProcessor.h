#pragma once

#include <memory>
#include <atomic>

#include "Frame.h"
#include "Processor.h"

class BlobProcessor: Processor {
private:
	
	template<typename T, typename K>
	bool hasElement(std::map<T,K> &map, int index) {
		return map.find(index) != map.end();
	}


public:
	typedef std::pair<std::shared_ptr<Frame>,std::shared_ptr<Frame>> __previous_current__;
	static std::map<int,__previous_current__> cache; //i want this to be std::uniq_ptr<__previous_current__> but error
	static std::map<int,__previous_current__> last_result;
	BlobProcessor();

	std::shared_ptr<Frame> run(std::shared_ptr<Frame> f);
	std::shared_ptr<Frame> findBlob(cv::Mat diff,std::shared_ptr<Frame> color);
	
};