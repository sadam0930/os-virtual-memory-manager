#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "pager.h" //algorithms
#include "pagetableentry.h"

#ifndef nullptr
#define nullptr __null
#endif

#define MAXFRAMES 64

using namespace std;

typedef enum readWrite {
	READ,
	WRITE
} readWrite;

// global
vector<int> randvals; 
unsigned int ofs = 0;
int myrandom(int size) { 
	int ranNum = randvals[ofs] % size;
	ofs++;
	if(ofs == randvals.size()-1){
		ofs = 0;
	}
	return ranNum; 
}

void initialize(string randfile){
	ifstream f;
	//read randfile
	f.open(randfile);
	if(f.is_open()){
		string line;
		//First line is the count of random numbers in the file
		getline(f, line); //throw it away
		//Each line in the file is a random number
		while(getline(f, line)){
			randvals.push_back(stoi(line, nullptr));
		}
	} else {
		cout << "Could not open file: " << randfile << endl;
		exit(1);
	}
	f.close();
}

void page_in(int frameIndex){

}

void zero(int frameIndex){

}

void map(PageTableEntry * pte, int frameIndex){

}

void update_pte(int readOrWrite, PageTableEntry * pte){

}

//begin memory management unit simulation
int main(int argc, char **argv){
	int opt;
	bool Oflag, Pflag, Fflag, Sflag, pflag, fflag, aflag = false;
	int numFrames = 0;
	Pager * pager;
	while ((opt = getopt (argc, argv, "a:o:f:")) != -1) {
        if (opt == 'a') {
        	//get algorithm type from optarg[0]
        	if(optarg[0] == 'N'){

        	} else if(optarg[0] == 'l'){

        	} else if(optarg[0] == 'r'){

        	} else if(optarg[0] == 'f'){
        		pager = new FIFO_Pager();
        	} else if(optarg[0] == 's'){

        	} else if(optarg[0] == 'c'){

        	} else if(optarg[0] == 'X'){

        	} else if(optarg[0] == 'a'){

        	} else if(optarg[0] == 'Y'){

        	} 
        }
        if (opt == 'o') {
        	for(int i = 0; i < strlen(optarg); i++){
        		switch(optarg[i]) {
					case 'O': Oflag = true; break;
					case 'P': Pflag = true; break;
					case 'F': Fflag = true; break;
					case 'S': Sflag = true; break;
					//below options are not graded
					case 'p': pflag = true; break;
					case 'f': fflag = true; break;
					case 'a': aflag = true; break;
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

	vector<PageTableEntry *> * pageTable = new vector<PageTableEntry *>(64); //maps virtual page to a frame. Assume 64 pages.
	vector<unsigned int> * frameTable = new vector<unsigned int>(numFrames); //inverse page table mapping frame to a virtual page
	vector<unsigned int> * framesInMemory = new vector<unsigned int>(); //keep track of frames in use
	if(numFrames <= 0) { numFrames = 32; };

	initialize(randfile);

	//start processing instructions from file
	ifstream f;
	f.open(filename);

	if(f.is_open()){
		string instruction;
		long long numInstruction = 0; //can reach over 1 million
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

						// //invalidate page that was removed
						// pageTable->at(frameTable->at(frameIndex))->present = false;

						if(Oflag){
							cout << count << ": UNMAP" << setfill(' ') << setw(4) << frameTable->at(frameIndex) << setfill(' ') << setw(4) << frameIndex << endl;
						}
					}

					if(pageTable->at(virPageNum)->pagedout == true){
						//page was swapped to disk; bring it back to the allocated frame
						page_in(frameIndex);
					} else {
						//page was not swapped; zero the frame
						zero(frameIndex);
						cout << numInstruction << ": ZERO" << setfill(' ') << setw(9) << frameIndex << endl;
					}
					//map virtual page to frame
					map(pageTable->at(virPageNum), frameIndex);
					cout << numInstruction << ": MAP" << setfill(' ') << setw(6) << virPageNum << setfill(' ') << setw(4) << frameIndex << endl;
				}
				//update page table entry based on operations
				update_pte(readOrWrite, pageTable->at(virPageNum));
			}
			numInstruction++;
		}

	} else {
		cout << "Could not open file: " << filename << endl;
		exit(1);
	}
	f.close();

	return 0;
}