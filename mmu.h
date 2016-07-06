#include "pagetableentry.h"

#ifndef nullptr
#define nullptr __null
#endif

using namespace std;

class mmu {
	protected:
		vector<PageTableEntry> * pageTable; //maps virtual page to a frame
		vector<unsigned int> * frameToPage; //inverse page table mapping frame to a virtual page
		vector<unsigned int> * frameTable; 
		int numFrames;
	public:
		mmu(int numFrames){
			this->frameTable = new vector<unsigned int>();
			this->numFrames = numFrames;
			this->pageTable = new vector<PageTableEntry>(64); //assume 64 virtual pages
			this->frameToPage = new vector<unsigned int>(numFrames);
		}
};