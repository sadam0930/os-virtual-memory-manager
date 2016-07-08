#include <vector>
#include "pagetableentry.h"

#ifndef nullptr
#define nullptr __null
#endif

//implementation will differ based on the scheduling algorithm
class Pager {
	public:
		Pager() {}
		virtual int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) =0;
};

/**************************************************************
differentiate how to how frames are allocated between algorithms
***************************************************************/

//First In First Out
class FIFO_Pager : public Pager {
	public:
		FIFO_Pager() : Pager() {}

		//get framenum at the front and push it to the end
		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum = framesInMemory->front();
			framesInMemory->erase(framesInMemory->begin());
			framesInMemory->push_back(frameNum);
			return frameNum;
		}
};