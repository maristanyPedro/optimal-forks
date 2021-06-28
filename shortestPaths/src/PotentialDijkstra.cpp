#include <algorithm>
#include <assert.h>

#include "../../graph/includes/graph.h"
#include "../../datastructures/includes/Label.h"

#include "../../shortestPaths/includes/AllToAll.h"

#include "../includes/PotentialDijkstra.h"

PotentialDijkstra::PotentialDijkstra(
        const Graph& G,
        const SubGraph& activeArcs,
        const std::vector<int>& arcCosts,
        const AllToAll& upperBounds,
        Node sourceId,
        Node targetId):
        G{G},
        activeArcs{activeArcs},
        arcCosts{arcCosts},
        upperBounds{upperBounds},
        sourceId{sourceId},
        targetId{targetId},
        result(this->G.nodesCount),
        permanent(this->G.nodesCount, false) {}

void PotentialDijkstra::run(
        std::pair<Node, Node> getArcOrientation(const Arc& a),
        const std::vector<std::size_t>& getNeighborhood(const Graph& G, Node n),
        bool respectsPotential(int dist_split_node, int dist_node_v, int dist_split_v),
        Node v) {
    this->insert(this->sourceId, 0, INVALID, INVALID);

    while (!this->H.empty()) {
        const Label minLabel{this->extractMin()};
        //Lazy deletion.
        if (!this->permanent[minLabel.n]) {
            this->makePermanent(minLabel);
            //Condition has no effect in o2a. Stops in o2o if minLabel is at t.
            if (this->stop(minLabel)) {
                break;
            }
            for (size_t arcId : getNeighborhood(this->G, minLabel.n)) {
                //for (size_t arcId : this->G.outgoingArcs(minLabel.n)) {
                if (this->isActive(arcId)) {
                    const Arc &a{this->G.arc(arcId)};
                    std::pair<Node, Node> arcOrientation = getArcOrientation(a);
                    if (!this->permanent[arcOrientation.second]) { //No need to propagate if a smaller label has been set.
                        if (respectsPotential(minLabel.c + this->arcCosts[arcId], this->upperBounds.distance(arcOrientation.second, v), this->upperBounds.distance(this->sourceId ,v))) {
                            this->insert(arcOrientation.second, minLabel.c + this->arcCosts[arcId],
                                         arcOrientation.first, arcId);
                        }
                    }
                }
            }
        }
    }
}

std::vector<const Arc*> PotentialDijkstra::getArcsToNode(std::size_t n, bool reverse) const {
    assert(this->permanent[n]); //If not, the node was not reached during the search or no search was yet performed.
    std::vector<const Arc *> path;
    const Label *l{&this->result[n]};
    while (l->predArc != INVALID) {
        path.push_back(&this->G.arc(l->predArc));
        l = &this->result[l->predNode];
    }
    if (reverse) {
        std::reverse(path.begin(), path.end());
    }
    return path;
}


Path PotentialDijkstra::getPathToNode(Node n, bool reverse) const {
    assert(this->permanent[n]); //If not, the node was not reached during the search or no search was yet performed.
    Path path;
    const Label* l{&this->result[n]};
    path.push_back(l->n);
    while (l->predArc != INVALID) {
        Node predNode{l->predNode};
        path.push_back(predNode);
        l = &this->result[predNode];
        assert(l->n == predNode);
    }
    if (reverse) {
        std::reverse(path.begin(), path.end());
    }
    return path;
}

int PotentialDijkstra::distance(std::size_t n) const {
    return this->result[n].c;
}

Label PotentialDijkstra::extractMin() {
    const Label l{this->H.top()};
    this->H.pop();

    //const Label l{*this->H.begin()};
    //this->H.erase(this->H.begin());
    return l;
}

void PotentialDijkstra::makePermanent(const Label& l) {
    this->permanent[l.n] = true;
    this->result[l.n] = l;
}

void PotentialDijkstra::printDistances() const {
    printf("SourceId: %lu\n", this->sourceId);
    for (size_t n = 0; n < this->G.nodesCount; ++n) {
        printf("(%lu, %d)\n", n, this->cost(n).c);
    }
}

bool potentialBeforeSplit(int dist_split_node, int dist_node_v, int dist_split_v) {
    return dist_split_node <= dist_node_v - dist_split_v;
}

bool potentialAfterSplit(int dist_split_node, int dist_node_v, int dist_split_v) {
    return dist_split_node >= dist_split_v - dist_node_v;
}