#include "../../graph/includes/graph.h"
#include "../includes/OneToAll.h"

#include "../includes/AllToAll.h"

#include <vector>
using namespace std;

AllToAll::AllToAll(const Graph &G, const std::vector<int>& arcCosts):
    G{G},
    arcCosts{arcCosts},
    dist{this->G.nodesCount} {}

void AllToAll::run() {
    vector<bool> activeArcs(this->G.arcsCount, true);
    for (size_t n = 0; n < this->G.nodesCount; ++n) {
        OneToAll dijkstra(G, activeArcs, this->arcCosts, n);
        dijkstra.run();
        dist[n] = dijkstra.getResult();
    }
}

void AllToAll::printDistanceMatrix() const {
    for (size_t i = 0; i < this->G.nodesCount; ++i) {
        for (size_t j = 0; j < this->G.nodesCount; ++j) {
            printf("%d\t", this->distance(i,j));
        }
        printf("\n");
    }
}
