#ifndef ONE_TO_ALL_
#define ONE_TO_ALL_

#include "Dijkstra.h"

class OneToAll : public Dijkstra {
public:
    OneToAll(const Graph& G, const std::vector<bool>& activeArcs, const std::vector<int>& arcCosts, std::size_t sourceId):
        Dijkstra(G, activeArcs, arcCosts, sourceId) {}

private:
    inline bool stop(const Label& l) const {
        return l.n == INVALID;
    }
};

#endif