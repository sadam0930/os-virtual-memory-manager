#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>

#include "pager.h"

#ifndef nullptr
#define nullptr __null
#endif

using namespace std;

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
	bool Oflag, Pflag, Fflag, Sflag = false;
	int numFrames;
	while ((opt = getopt (argc, argv, "a:o:f:")) != -1) {
        if (opt == 'a') {
        	//get algorithm type from optarg[0]
        	if(optarg[0] == 'N'){

        	} else if(optarg[0] == 'l'){

        	} else if(optarg[0] == 'r'){

        	} else if(optarg[0] == 'f'){

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
					case 'O':
						Oflag = true;
					case 'P':
						Pflag = true;
					case 'F':
						Fflag = true;
					case 'S':
						Sflag = true;
				}
        	}
        }
        if (opt == 'f') {
        	sscanf(optarg, "%d", &numFrames);
        }
    }

	string filename = argv[optind];
	string randfile = argv[optind+1];

	initialize(randfile);

	//start processing instructions from file
	ifstream f;
	f.open(filename);
	if(f.is_open()){
		string line;
		//Each line in the file is an instruction
		while(getline(f, line)){
			istringstream iss(line);
			if(line[0] == "#"){
				//ignore commented lines
				continue;
			} else {
				//First int is read (0) or write (1)
				//Second int is virtual page number
				int readWrite, virPageNum = 0;
				iss >> readWrite >> virPageNum;


			}
		}

	} else {
		cout << "Could not open file: " << filename << endl;
		exit(1);
	}
	f.close();

	return 0;
}