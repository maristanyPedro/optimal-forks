# optimal-forks

This repository contains the code needed to run Algorithm 1 as presented in the paper:
DOI:

Given a directed graph with a set of source nodes, this program identifies for each source node s, which arcs in the graph are guaranteed to not lie on any shortest path from s to another node in the graph.

COMPILING AND RUNNING THE CODE

This code is made publicly available as a CMake project. It has been tested using Ubuntu 20.04 LTS and CMAKE 3.20.05. The code is organized in subdirectories, each of them being linked to the main executable as a library. This approach could cause
problems when trying to configure the program using a Windows operating systems. 

To compile the project, create a subdirectory 'build' and change into it. From there, call

    cmake .. -DCMAKE_BUILD_TYPE=Release

to configure the program's makefile as it was used for the paper originally. Then, call

    make

A new file 'forkOptimizer.o' will be generated in the same (the 'build' one) directory. If everything worked well, you should be able to call the program on the toy.txt instance that you can find in the 'instances' directory. This is done by calling

    ./forkOptimizer.o ../instances/toy.txt

