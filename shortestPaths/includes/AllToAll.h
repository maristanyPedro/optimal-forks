#ifndef ALL_TO_ALL_H
#define ALL_TO_ALL_H

#include "../../datastructures/includes/Label.h"

class Graph;

class AllToAll{
public:
    typedef std::vector<std::vector<Label>> DistanceMatrix;
    AllToAll() = delete;
    AllToAll(const Graph& G, const std::vector<int>& arcCosts);

    void run();

    int distance(std::size_t node1, std::size_t node2) const {
        return this->dist[node1][node2].c;
    }

    void printDistanceMatrix() const;

private:
    const Graph& G;
    const std::vector<int>& arcCosts;

    DistanceMatrix dist;
};

#endif