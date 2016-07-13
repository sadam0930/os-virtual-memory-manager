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

		virtual void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory) =0;
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

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
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

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};

//Second Chance
class SC_Pager : public Pager {
	public:
		SC_Pager(){}

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum;
			while(true){
				frameNum = framesInMemory->front();
				framesInMemory->erase(framesInMemory->begin());
				framesInMemory->push_back(frameNum);
				if(pageTable->at(frameTable->at(frameNum))->referenced == false){
					break;
				}
				pageTable->at(frameTable->at(frameNum))->referenced = false;
			}
			
			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};

//Clock based on physical frames
class fClock_Pager : public Pager {
	int pointer;

	public:
		fClock_Pager(){ pointer = 0; }

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum;
			
			while(true){
				frameNum = framesInMemory->at(pointer);
				if(pageTable->at(frameTable->at(frameNum))->referenced == false){
					break;
				}
				pageTable->at(frameTable->at(frameNum))->referenced = false;
				pointer = (pointer+1) % framesInMemory->size();
			}
			pointer = (pointer+1) % framesInMemory->size();
			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};

//Clock based on virtual pages
class vClock_Pager : public Pager {
	int pointer;

	public:
		vClock_Pager(){ pointer = 0; }

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum;
			
			while(true){
				if(pageTable->at(pointer)->present == true){
					if(pageTable->at(pointer)->referenced == false){
						frameNum = pageTable->at(pointer)->frameIndex;
						break;
					} else{
						pageTable->at(pointer)->referenced = false;
					}
				}
				pointer = (pointer+1) % pageTable->size();
			}
			pointer = (pointer+1) % pageTable->size();
			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};

//will use framesInMemory to track least recently used page
class LRU_Pager : public Pager {
	public:
		LRU_Pager(){}

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum = framesInMemory->front();
			framesInMemory->erase(framesInMemory->begin());
			framesInMemory->push_back(frameNum);
			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory) {
			for(unsigned int i=0; i < framesInMemory->size(); i++){
				if(framesInMemory->at(i) == frameIndex){
					framesInMemory->erase(framesInMemory->begin()+i);
					framesInMemory->push_back(frameIndex);
				}
			}
		}
};

/*
Not Recently Used:
Picks a page at random based on the lowest class it falls into
3. referenced, modified
2. referenced, not modified
1. not referenced, modified
0. not referenced, not modified
*/
class NRU_Pager : public Pager {
	int numReplacements;
	
	public:
		NRU_Pager(){ numReplacements = 0; }

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum;
			std::vector<PageTableEntry *> pclass[4];
			numReplacements++;

			//categorize all of the virtual pages
			for(unsigned int i=0; i < pageTable->size(); i++){
				PageTableEntry * pte = pageTable->at(i);

				if(pte->present == true){
					if(pte->referenced == false && pte->modified == false){
						pclass[0].push_back(pte);
					} else if(pte->referenced == false && pte->modified == true){
						pclass[1].push_back(pte);
					} else if(pte->referenced == true && pte->modified == false){
						pclass[2].push_back(pte);
					} else if(pte->referenced == true && pte->modified == true){
						pclass[3].push_back(pte);
					}
				}
			}

			//choose page to replace starting from lowest class
			for(int i=0; i < 4; i++){
				if(!pclass[i].empty()){
					int p_index = rando->getRandom() % pclass[i].size();
					frameNum = pclass[i][p_index]->frameIndex;
					break; //got it, now leave
				}
			}

			//reset r bit per every 10 page faults
			if(numReplacements == 10){
				numReplacements = 0;
				for(unsigned int i=0; i < pageTable->size(); i++){
					if(pageTable->at(i)->present == true){
						pageTable->at(i)->referenced = false;
					}
				}
			}

			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};

//Aging based on physical frames
class fAging_Pager : public Pager {
	std::vector<unsigned int> ages;

	public:
		fAging_Pager(){}

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum = 0;
			//initialize age vector the first time with starting age of 0
			if(ages.size() == 0){
				ages = std::vector<unsigned int>(framesInMemory->size(), 0);
			}

			//traverse age vector to find lowest age; push back age with right shift
			PageTableEntry * pte;
			for(unsigned int i=0; i < framesInMemory->size(); i++){
				pte = pageTable->at(frameTable->at(i));
				ages.at(i) = ages.at(i) >> 1 | pte->referenced << 31;
				pte->referenced = 0;

				if(ages.at(i) < ages.at(frameNum)){
					frameNum = i;
				}
			}

			//reset age
			ages[frameNum] = 0;

			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};

//Aging based on virtual pages
class vAging_Pager : public Pager {
	std::vector<unsigned int> ages;

	public:
		vAging_Pager(){}

		int allocate_frame(std::vector<PageTableEntry *> * pageTable, std::vector<unsigned int> * frameTable, std::vector<unsigned int> * framesInMemory) {
			int frameNum;
			
			//initialize age vector the first time with starting age of 0
			if(ages.size() == 0){
				ages = std::vector<unsigned int>(pageTable->size(), 0);
			}

			//traverse age vector to find lowest age; push back age with right shift
			PageTableEntry * pte;
			int ageIdx = 0;

			//first start from the lowest present entry
			for(unsigned int i=0; i < pageTable->size(); i++){
				pte = pageTable->at(i);
				if(pte->present == true){
					ageIdx = i;
					break;
				}
			}

			//now find lowest age
			for(unsigned int i=ageIdx; i < pageTable->size(); i++){
				pte = pageTable->at(i);
				ages.at(i) = ages.at(i) >> 1 | pte->referenced << 31;
				
				if(pte->present == true){
					pte->referenced = false;

					if(ages.at(i) < ages.at(ageIdx)){
						ageIdx = i;
					}
				}

			}
			frameNum = pageTable->at(ageIdx)->frameIndex;

			//reset age
			ages[ageIdx] = 0;

			return frameNum;
		}

		void update_frames(int frameIndex, std::vector<unsigned int> * framesInMemory){}
};