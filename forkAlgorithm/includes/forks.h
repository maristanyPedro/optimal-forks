#ifndef FORKS_H_
#define FORKS_H_

#include "../../shortestPaths/includes/AllToAll.h"

class Graph;

/**
 * Implementation of Algorithm 1, as introduced in the paper:
 * DOI:
 * It is used to decide whether an arc in a directed graph with interval data lies on some shortest path from a given
 * source vertex s.
 */
class ForkAlgorithm {
public:
    ForkAlgorithm(const Graph& G, std::size_t sourceId, const AllToAll& ubDistances);
    ForkAlgorithm(const ForkAlgorithm& fa) = default;

    int run(std::size_t v, std::size_t w) const;
private:
    const Graph& G;
    std::size_t sourceId;

    const AllToAll& upperBounds;
};

#endif