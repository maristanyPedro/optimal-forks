#ifndef WEAK_ARC_FINDER_H_
#define WEAK_ARC_FINDER_H_

#include <vector>

#include "../../shortestPaths/includes/AllToAll.h"

class Graph;
struct Arc;

class WeakArcFinder {
public:
    WeakArcFinder(const Graph& G);

    /**
     * Iterates over all arcs in G and decides whether there are weak using the fork calculator.
     * @param sourceId The id of the source s for the s-v-w-scenario problems.
     * @return The set of s-weak arcs in G.
     */
    std::vector<const Arc*> collectWeakArcs(const Node sourceId) const;

private:
    const Graph& G;
    /**
     * All pairs shortest Distance matrix w.r.t. the upper bound costs u.
     */
    AllToAll upperBounds;
};

#endif