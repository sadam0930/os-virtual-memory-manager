# os-virtual-memory-manager
-------------

# To compile:
`make`

without makefile : g++ -std=c++0x -Wall -o mmu mmu.cpp

# To run:
`mkdir myoutput`
`./runit_genclass.sh myoutput ./mmu`


Synopsis:  
This is a simulation of an operating system Memory Management Unit. It takes an input file with information about which virtual page is being referenced (Read or Written) and uses several algorithms to choose which physical memory frame to map to: First Come First Serve, Second Chance, Random, Not Recently Used, Least Recently Used, Aging