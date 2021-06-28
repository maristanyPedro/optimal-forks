#ifndef POT_DIJKSTRA_H_
#define POT_DIJKSTRA_H_

#include <functional>
#include <set>
#include <queue>
#include <vector>

#include "../../datastructures/includes/Label.h"

class Graph;
class AllToAll;

/**
 * Dijkstra Algorithm for one to one and one to all scenarios on directed graphs. The implementation follows a lazy update
 * strategy in the priority queue: no keys are decreased; instead, when extracting a label, we check if the corresponding
 * node was made permanent already.
 */
class PotentialDijkstra {
public:
    //using Heap = std::set<Label>;
    using Heap = std::priority_queue<Label>;
    PotentialDijkstra() = delete;
    PotentialDijkstra(
            const Graph& G,
            const SubGraph& activeArcs,
            const std::vector<int>& arcCosts,
            const AllToAll& upperBounds,
            std::size_t sourceId, std::size_t targetId);

    void run(
            std::pair<Node, Node> getArcOrientation(const Arc& a),
            const std::vector<std::size_t>& getNeighborhood(const Graph& G, Node n),
            bool respectsPotential(int dist_split_node, int dist_node_v, int dist_split_v),
            Node v);

    inline std::vector<Label> getResult() const {
        return this->result;
    }

    std::vector<const Arc*> getArcsToNode(std::size_t n, bool reverse = true) const;

    Path getPathToNode(const Node n, bool reverse = true) const;

    int distance(Node n) const;

    inline const Label& cost(const Node nodeId) const {
        return this->result[nodeId];
    }

    inline bool isActive(std::size_t arcId) const {
        return this->activeArcs[arcId];
    }

    inline bool reached(Node n) const {
        return this->permanent[n];
    }

    void printDistances() const;

    const Graph& getGraph() const;
private:
    const Graph& G;
    const std::vector<bool>& activeArcs;
    const std::vector<int>& arcCosts;
    const AllToAll& upperBounds;
    const size_t sourceId;
    const size_t targetId;

    Heap H;
    std::vector<Label> result;
    std::vector<bool> permanent;


private:
    bool stop(const Label& l) const {
        return l.n == targetId;
    }

    Label extractMin();

    void makePermanent(const Label& l);

    inline void insert(std::size_t node, CostType c, std::size_t predNode, const std::size_t predArcId) {
        this->H.emplace(node, c, predNode, predArcId);
    }
};

inline const Graph& PotentialDijkstra::getGraph() const {
    return this->G;
}

bool potentialBeforeSplit(int dist_split_node, int dist_node_v, int dist_split_v);
bool potentialAfterSplit(int dist_split_node, int dist_node_v, int dist_split_v);

#endif