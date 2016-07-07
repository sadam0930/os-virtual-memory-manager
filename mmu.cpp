#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>

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
				cout<<"Error: number of frames is too large (max=64)"<<endl;
				exit(1);
			}
        }
    }

	string filename = argv[optind];
	string randfile = argv[optind+1];

	initialize(randfile);

	vector<PageTableEntry> * pageTable = new vector<PageTableEntry>(64); //maps virtual page to a frame. Assume 64 pages.
	vector<unsigned int> * frameTable = new vector<unsigned int>(numFrames); //inverse page table mapping frame to a virtual page
	vector<unsigned int> * framesInMemory = new vector<unsigned int>(); //keep track of frames in use
	if(numFrames <= 0) { numFrames = 32 };
	
	//start processing instructions from file
	ifstream f;
	f.open(filename);

	if(f.is_open()){
		string instruction;
		//Each line in the file is an instruction
		while(getline(f, instruction)){
			istringstream iss(instruction);
			if(instruction[0] == "#"){
				//ignore commented lines
				continue;
			} else {
				//First int is read (0) or write (1)
				//Second int is virtual page number
				int readOrWrite, virPageNum = 0;
				iss >> readOrWrite >> virPageNum;

				// cout << readOrWrite << " " << virPageNum << endl; //test
			}
		}

	} else {
		cout << "Could not open file: " << filename << endl;
		exit(1);
	}
	f.close();

	return 0;
}