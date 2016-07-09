#include <vector>
#include <iostream>
#include "pagetableentry.h"
#include "random.h"

#ifndef nullptr
#define nullptr __null
#endif

//implementation will differ based on the scheduling algorithm
class Pager {
	protected:
	Randval * rando; //only for LRU and Random Pagers

	public:
		Pager() {}
		virtual int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) =0;

		void setRando(Randval * rando) {
			this->rando = rando;
		}
};

/**************************************************************
differentiate how to how frames are allocated between algorithms
***************************************************************/

//First In First Out
class FIFO_Pager : public Pager {
	public:
		FIFO_Pager(){}

		//get frameNum at the front and push it to the end
		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum = framesInMemory->front();
			framesInMemory->erase(framesInMemory->begin());
			framesInMemory->push_back(frameNum);
			return frameNum;
		}
};

//Random selection
class RAN_Pager : public Pager {
	public:
		RAN_Pager(){}

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum = (rando->getRandom() % framesInMemory->size());
			framesInMemory->erase(framesInMemory->begin() + frameNum);
			framesInMemory->push_back(frameNum);
			return frameNum;
		}
};