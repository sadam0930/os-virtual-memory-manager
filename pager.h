#ifndef pager_header
#define pager_header

#include "frame.h"

#ifndef nullptr
#define nullptr __null
#endif

class Pager {
	public:
		Pager();
		virtual int allocate_frame(vector<PageTableEntry> * pageTable, vector<unsigned int> * frameTable, vector<unsigned int> * framesInMemory); 
};

class FIFO_Pager : public Pager {
	public:
		FIFO_Pager();

		int allocate_frame(vector<PageTableEntry> * pageTable, vector<unsigned int> * frameTable, vector<unsigned int> * framesInMemory) {
			int frameNum = -1;
			
			return frameNum;
		}
};

#endif