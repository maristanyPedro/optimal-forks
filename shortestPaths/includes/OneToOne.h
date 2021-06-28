#ifndef ONE_TO_ONE_
#define ONE_TO_ONE_

#include "Dijkstra.h"

class OneToOne : public Dijkstra {
public:
    OneToOne(const Graph& G, const std::vector<bool>& activeArcs, const std::vector<int>& arcCosts, std::size_t sourceId, std::size_t targetId):
        Dijkstra(G, activeArcs, arcCosts, sourceId),
        targetId{targetId} {}
private:

   const std::size_t targetId;

private:
    inline bool stop(const Label& l) const {
        return l.n == this->targetId;
    }
};

#endif