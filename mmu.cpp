#include <stdio.h>
#include <string>

#include "pager.h"

using namespace std;

//begin memory management unit simulation
int main(int argc, char **argv){
	int opt;
	while ((opt = getopt (argc, argv, "vs:")) != -1) {
		//instantiate a scheduler based on the -s option
        if (opt == 'a') {}
        if (opt == 'o') {}
        if (opt == 'f') {}
    }

	string filename = argv[optind];
	string randfile = argv[optind+1];

	return 0;
}