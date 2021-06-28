#include <assert.h>
#include <chrono>  // for high_resolution_clock
#include <iostream>

#include "../../graph/includes/graph.h"
#include "../../shortestPaths/includes/OneToAll.h"
#include "../../shortestPaths/includes/PotentialDijkstra.h"

#include "../includes/forks.h"

using namespace std;

ForkAlgorithm::ForkAlgorithm(const Graph &G, std::size_t sourceId, const AllToAll& ubDistances):
    G{G},
    sourceId{sourceId},
    upperBounds{ubDistances} {}

int ForkAlgorithm::run(size_t v, size_t w) const {
    int bestForkValue = std::numeric_limits<int>::min();
    SubGraph allArcs(this->G.arcsCount, true);
    for (size_t splitId = 0; splitId < this->G.nodesCount; ++splitId) {
        PotentialDijkstra split_to_source(G, allArcs, G.getLowerBoundCosts(), this->upperBounds, splitId, this->sourceId);
        split_to_source.run(backwardArc, backwardStar, potentialBeforeSplit, v);
        if (!split_to_source.reached(this->sourceId)) {
            continue;
        }
        PotentialDijkstra split_to_w(G, allArcs, G.getLowerBoundCosts(), this->upperBounds, splitId, w);
        split_to_w.run(forwardArc, forwardStar, potentialAfterSplit, v);
        if (!split_to_w.reached(w)) {
            continue;
        }
        std::vector<const Arc *> q_arcs = split_to_source.getArcsToNode(this->sourceId, false);
        std::vector<const Arc *> split_w_arcs = split_to_w.getArcsToNode(w);
        q_arcs.insert(q_arcs.end(), split_w_arcs.begin(), split_w_arcs.end());
        const std::vector<int> costScenario = buildCostScenario(this->G.getUpperBoundCosts(), q_arcs);
        OneToAll q_dijkstra(G, allArcs, costScenario, this->sourceId);
        q_dijkstra.run();
        if (split_to_source.distance(this->sourceId) + split_to_w.distance(w) == q_dijkstra.distance(w)) {
            int forkValue = q_dijkstra.distance(v) - q_dijkstra.distance(w);
            if (forkValue > bestForkValue) {
                bestForkValue = forkValue;
            }
        }
    }
    return bestForkValue;
}
