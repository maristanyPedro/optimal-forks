#ifndef LEX_DIJKSTRA_H_
#define LEX_DIJKSTRA_H_

#include <functional>
#include <set>
#include <queue>
#include <vector>

#include "../../datastructures/includes/Label.h"
class Graph;


/**
 * Dijkstra Algorithm for one to one and one to all scenarios on directed graphs. The implementation follows a lazy update
 * strategy in the priority queue: no keys are decreased; instead, when extracting a label, we check if the corresponding
 * node was made permanent already.
 * Abstract Class. The function "stop" always returns false in the one to all case. In the one to one case, the function
 * checks if the label that was newly made permanent corresponds to a path to the target node.
 */
class Dijkstra {
public:
    //using Heap = std::set<Label>;
    using Heap = std::priority_queue<Label>;
    Dijkstra() = delete;
    Dijkstra(const Graph& G, const SubGraph& activeArcs, const std::vector<int>& arcCosts, std::size_t sourceId);

    void run();

    inline std::vector<Label> getResult() const {
        return this->result;
    }

    std::vector<const Arc*> getArcsToNode(std::size_t n) const;

    Path getPathToNode(const Node n) const;

    int distance(Node n) const;

    inline const Label& cost(const Node nodeId) const {
        return this->result[nodeId];
    }

    inline bool isActive(std::size_t arcId) const {
        return this->activeArcs[arcId];
    }

    void printDistances() const;

    const Graph& getGraph() const;
private:
    const Graph& G;
    const std::vector<bool>& activeArcs;
    const std::vector<int>& arcCosts;
    const size_t sourceId;

    Heap H;
    std::vector<Label> result;
    std::vector<bool> permanent;


private:
    virtual bool stop(const Label& l) const = 0;

    Label extractMin();

    void makePermanent(const Label& l);

    inline void insert(std::size_t node, CostType c, std::size_t predNode, const std::size_t predArcId) {
        this->H.emplace(node, c, predNode, predArcId);
    }
};

inline const Graph& Dijkstra::getGraph() const {
    return this->G;
}

#endif