#ifndef ran_header
#define ran_header

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>


class Randval {
	std::vector<int> randvals;

	public:
		unsigned int ofs;
		Randval(std::string randfile){
			ofs = 0;
			std::ifstream f;

			//read randfile
			f.open(randfile);
			if(f.is_open()){
				std::string line;
				//First line is the count of random numbers in the file
				getline(f, line); //throw it away
				//Each line in the file is a random number
				while(getline(f, line)){
					randvals.push_back(stoi(line, nullptr));
				}
			} else {
				std::cout << "Could not open file: " << randfile << std::endl;
				exit(1);
			}
			f.close();
		}

		int getRandom(){
			// std::cout<<"randvals = "<< randvals.front() <<std::endl;
			int ranNum = randvals[ofs];
			ofs++;
			if(ofs == randvals.size()-1){
				ofs = 0;
			}
			return ranNum; 
		}
};

#endif