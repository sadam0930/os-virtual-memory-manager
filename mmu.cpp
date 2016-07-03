#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>

#include "pager.h"

using namespace std;

// global
vector<int> randvals; 
unsigned int ofs = 0;
int myrandom(int burst) { 
	int ranNum = 1 + (randvals[ofs] % burst);
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
	while ((opt = getopt (argc, argv, "a:o:f:")) != -1) {
		//instantiate a scheduler based on the -s option
        if (opt == 'a') {}
        if (opt == 'o') {}
        if (opt == 'f') {}
    }

	string filename = argv[optind];
	string randfile = argv[optind+1];

	initialize(randfile);

	return 0;
}