#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "pager.h" //algorithms
#include "pagetableentry.h"

#ifndef nullptr
#define nullptr __null
#endif

#define MAXFRAMES 64
#define MAXPAGES 64

using namespace std;

typedef enum readWrite {
	READ,
	WRITE
} readWrite;

//global
Randval * rando;

void initialize(vector<PageTableEntry *> * pageTable){
	//initialize pageTable
	for(int i = 0; i < MAXPAGES; i++){
		pageTable->push_back(new PageTableEntry());
	}
}

void page_in(int frameIndex){

}

void page_out(vector<PageTableEntry *> * pageTable, vector<unsigned int> * frameTable, int frameIndex){
	pageTable->at(frameTable->at(frameIndex))->pagedout = true;
	pageTable->at(frameTable->at(frameIndex))->modified = false;
}

void zero(int frameIndex){

}

void map(PageTableEntry * pte, int virPageNum, int frameIndex, vector<unsigned int> * frameTable){
	pte->present = true;
	pte->frameIndex = frameIndex;
	frameTable->at(frameIndex) = virPageNum;
}

void unmap(vector<PageTableEntry *> * pageTable, vector<unsigned int> * frameTable, int frameIndex){
	//invalidate page that was removed
	pageTable->at(frameTable->at(frameIndex))->present = false;
}

void update_pte(int readOrWrite, PageTableEntry * pte){
	pte->referenced = true;
	if(readOrWrite == WRITE){
		pte->modified = true;
	}
}

//begin memory management unit simulation
int main(int argc, char **argv){
	int opt;
	int numFrames = 0;
	Pager * pager;

	//can reach over 1 million - using 64 bit counters
	long long numInstruction, umCount, mCount, oCount, iCount, zCount, totalCost;
	bool Oflag, Pflag, Fflag, Sflag, pflag, fflag, aflag;

	numInstruction = umCount = mCount = oCount = iCount = zCount = totalCost = 0;
	Oflag = Pflag = Fflag = Sflag = pflag = fflag = aflag = false;

	while ((opt = getopt (argc, argv, "a:o:f:")) != -1) {
        if (opt == 'a') {
        	//get algorithm type from optarg[0]
        	if(optarg[0] == 'N'){ pager = new NRU_Pager(); } 
        	else if(optarg[0] == 'l'){ pager = new LRU_Pager(); } 
        	else if(optarg[0] == 'r'){ pager = new RAN_Pager(); } 
        	else if(optarg[0] == 'f'){ pager = new FIFO_Pager(); } 
    		else if(optarg[0] == 's'){ pager = new SC_Pager(); } 
        	else if(optarg[0] == 'c'){ pager = new fClock_Pager(); } 
        	else if(optarg[0] == 'X'){ pager = new vClock_Pager(); } 
        	else if(optarg[0] == 'a'){ pager = new fAging_Pager(); } 
        	else if(optarg[0] == 'Y'){ } 
        }
        if (opt == 'o') {
        	for(int i = 0; i < strlen(optarg); i++){
        		switch(optarg[i]) {
					case 'O': Oflag = true; break;
					case 'P': Pflag = true; break;
					case 'F': Fflag = true; break;
					case 'S': Sflag = true; break;
					//below options are not graded
					// case 'p': pflag = true; break;
					// case 'f': fflag = true; break;
					// case 'a': aflag = true; break;
				}
        	}
        }
        if (opt == 'f') {
        	sscanf(optarg, "%d", &numFrames);
        	if (numFrames > MAXFRAMES){
				cout << "Error: number of frames is too large (max=64)" << endl;
				exit(1);
			}
        }
    }

	string filename = argv[optind];
	string randfile = argv[optind+1];

	if(numFrames <= 0) { numFrames = 32; };
	vector<PageTableEntry *> * pageTable = new vector<PageTableEntry *>(); //maps virtual page to a frame. Assume 64 pages.
	vector<unsigned int> * frameTable = new vector<unsigned int>(numFrames); //inverse page table mapping frame to a virtual page
	vector<unsigned int> * framesInMemory = new vector<unsigned int>(); //keep track of frames in use
	initialize(pageTable);
	pager->setRando(new Randval(randfile));

	//start processing instructions from file
	ifstream f;
	f.open(filename);

	if(f.is_open()){
		string instruction;

		//Each line in the file is an instruction
		while(getline(f, instruction)){
			istringstream iss(instruction);
			if(instruction[0] == '#'){
				//ignore commented lines
				continue;
			} else {
				//First int is read (0) or write (1); Second int is virtual page number
				int readOrWrite, virPageNum = 0;
				iss >> readOrWrite >> virPageNum;

				if(Oflag){
					cout << "==> inst: " << readOrWrite << " " << virPageNum << endl;
				}

				if (pageTable->at(virPageNum)->present == false){
					//page is not in memory, get memory frame to map to
					int frameIndex;

					if(framesInMemory->size() < numFrames){
						//free frames available
						frameIndex = framesInMemory->size();
						framesInMemory->push_back(frameIndex);
					} else {
						//all frames have been written to; pager algorithm picks one to overwrite
						frameIndex = pager->allocate_frame(pageTable, frameTable, framesInMemory);
						unmap(pageTable, frameTable, frameIndex);
						umCount++;

						if(Oflag){
							cout << numInstruction << ": UNMAP" << setfill(' ') << setw(4) << frameTable->at(frameIndex) << setfill(' ') << setw(4) << frameIndex << endl;
						}

						//if page was modified, need to swap it to disk
						if(pageTable->at(frameTable->at(frameIndex))->modified == true){
							page_out(pageTable, frameTable, frameIndex);
							oCount++;

							if(Oflag){
								cout << numInstruction << ": OUT" << setfill(' ') << setw(6) << frameTable->at(frameIndex) << setfill(' ') << setw(4) << frameIndex << endl;
							}
						}
					}

					//map page from instruction
					if(pageTable->at(virPageNum)->pagedout == true){
						//page was swapped to disk; bring it back to the allocated frame
						page_in(frameIndex);
						iCount++;

						if(Oflag){
							cout << numInstruction << ": IN" << setw(7) << virPageNum << setfill(' ') << setw(4) << frameIndex << endl;
						}
					} else {
						//page was not swapped; zero the frame
						zero(frameIndex);
						zCount++;

						if(Oflag){
							cout << numInstruction << ": ZERO" << setfill(' ') << setw(9) << frameIndex << endl;
						}
					}
					//map virtual page to frame
					map(pageTable->at(virPageNum), virPageNum, frameIndex, frameTable);
					mCount++;
					
					if(Oflag){
						cout << numInstruction << ": MAP" << setfill(' ') << setw(6) << virPageNum << setfill(' ') << setw(4) << frameIndex << endl;
					}
				} else {
					//page already present
					pager->update_frames(pageTable->at(virPageNum)->frameIndex, framesInMemory); //only used by LRU
				}
				//update page table entry based on operations
				update_pte(readOrWrite, pageTable->at(virPageNum));
			}
			numInstruction++;
		}

		//Print summary info based on flags
		if(Pflag){
			//print pagetable
			for(int i=0; i < pageTable->size(); i++){
				PageTableEntry * pte = pageTable->at(i);
				if(pte->present == true){
					cout << i << ":";
					(pte->referenced == true)? cout << "R" : cout << "-";
					(pte->modified == true)? cout << "M" : cout << "-";
					(pte->pagedout == true)? cout << "S" : cout << "-";
				} else {
					(pte->pagedout == true)? cout << "#" : cout << "*";
				}
				cout << " " ;
			}
			cout << endl;
		}

		if(Fflag){
			for(int i=0; i < frameTable->size(); i++){
				cout << frameTable->at(i) << " ";			
			}
			cout << endl;
		}

		if(Sflag){
			totalCost = (mCount + umCount)*400 + (iCount + oCount)*3000 + (zCount)*150 + numInstruction;
			cout << "SUM " << numInstruction << " U=" << umCount << " M=" << mCount << " I=" << iCount << " O=" << oCount << " Z=" << zCount << " ===> " << totalCost << endl;
		}

	} else {
		cout << "Could not open file: " << filename << endl;
		exit(1);
	}
	f.close();

	return 0;
}