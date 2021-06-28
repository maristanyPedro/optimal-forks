#include <chrono>  // for high_resolution_clock

#include <iostream>
#include <valgrind/callgrind.h>

#include "../graph/includes/graph.h"
#include "../forkAlgorithm/includes/WeakArcFinder.h"


using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("The program is meant to get the filepath for a graph as its argument.\n");
        exit(1);
    }
    //printf("FORK;%s\n", argv[1]);
    Graph G{argv[1]};
    //cout << "Start waf" << endl;
    WeakArcFinder waf(G);
    for (Node sourceNode : G.originNodes) {
          std::vector<const Arc*> weakArcs = waf.collectWeakArcs(sourceNode);
          //G.printSubGraph(weakArcs, sourceNode);
    }
}
