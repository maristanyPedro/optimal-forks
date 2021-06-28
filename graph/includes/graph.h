#ifndef GRAPH_H_
#define GRAPH_H_

#include <memory> //for unique_ptr
#include <map> //To translate the original node ids to 0-n.
#include <string>
#include <vector>

#include "../../definitions/includes/constants.h"

typedef std::size_t Node;

struct Arc {
    friend class Graph;
    Arc();
    Arc(const size_t arcId, const Node sourceId, const Node targetId, const int c1, const int c2);

    void print() const;

    size_t arcId;

    Node sourceId;
    Node targetId;

    int lowerBound;
    int upperBound;
};

std::pair<Node, Node> forwardArc(const Arc& a);
std::pair<Node, Node> backwardArc(const Arc& a);

typedef std::vector<Node> Path;

class NodeAdjacency {
    public:
        NodeAdjacency();
        NodeAdjacency(const std::size_t nid);
    //private:
        Node id;
        std::vector<std::size_t> incomingArcIds;
        std::vector<std::size_t> outgoingArcIds;
};

typedef std::vector<bool> SubGraph;

/**
 * Implementation of a directed graph using node-arc adjacency vectors.
 */
class Graph {
    public:
        Graph(const std::string& filename);

        const std::vector<std::size_t>& outgoingArcs(const std::size_t nodeId) const;
        const std::vector<std::size_t>& incomingArcs(const std::size_t nodeId) const;

        const Arc& arc(const size_t arcId) const;
        const NodeAdjacency& node(const Node nodeId) const;

        void disableNode(const Node n, SubGraph& sg) const;

        const std::vector<int>& getLowerBoundCosts() const;
        const std::vector<int>& getUpperBoundCosts() const;

        void printNodeInfo(const Node nodeId) const;
        void printArcs(const std::vector<size_t>& arcIds) const;
        void printOriginalArc(const size_t arcId) const;
        void printSubGraph(const std::vector<const Arc*> sg, const Node sourceNode) const;
        std::size_t getOriginalId(const Node n) const;

        std::string getName() const;

    std::size_t nodesCount;
    std::size_t arcsCount;
    std::vector<Node> originNodes;

    private: //Members
        std::vector<NodeAdjacency> nodes;
        std::vector<Arc> arcs;
        std::vector<int> lowerBounds;
        std::vector<int> upperBounds;

        std::vector<int> originalIds; //0-n mapped to originalIds.
        std::map<std::size_t, std::size_t> newIds;

        const std::string name;
    private: //Functions
        void readGraph(const std::string& filename);

        /**
        * Checks if the node with the input nodeId has already been added.
        *
        * This function is used during the arcwise creation of the graph to check if the
        * source or the target node of a new arc are already in the graph.
        * @param nodeId
        * @return
        */
        bool exists(Node nodeId) const;

    std::vector<int> buildCostVector(int getKey(const Arc&)) const;
};

inline bool Graph::exists(Node nodeId) const {
    return this->newIds.find(nodeId) != this->newIds.end();
}

inline std::size_t Graph::getOriginalId(const Node n) const {
    return this->originalIds[n];
}

inline const std::vector<int>& Graph::getLowerBoundCosts() const {
    return this->lowerBounds;
}

inline const std::vector<int>& Graph::getUpperBoundCosts() const {
    return this->upperBounds;
}

inline std::string Graph::getName() const {
    return this->name;
}

const std::vector<std::size_t>& forwardStar(const Graph& G, const std::size_t nodeId);

const std::vector<std::size_t>& backwardStar(const Graph& G, const std::size_t nodeId);

std::vector<int> buildCostScenario(const std::vector<int>& ubCosts, const std::vector<const Arc*>& lbArcs);

inline const Arc& Graph::arc(const size_t arcId) const {
    return this->arcs[arcId];
}

inline const NodeAdjacency& Graph::node(const Node nodeId) const {
    return this->nodes[nodeId];
}

void split(const std::string& s, char delim, std::vector<std::string>& elems);

std::vector<std::string> split(const std::string& s, char delim);

#endif
