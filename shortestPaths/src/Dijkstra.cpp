#include <algorithm>
#include <assert.h>

#include "../../graph/includes/graph.h"
#include "../../datastructures/includes/Label.h"

#include "../includes/Dijkstra.h"

Dijkstra::Dijkstra(const Graph& G, const SubGraph& activeArcs, const std::vector<int>& arcCosts, std::size_t sourceId):
        G{G},
        activeArcs{activeArcs},
        arcCosts{arcCosts},
        sourceId{sourceId},
        result(this->G.nodesCount),
        permanent(this->G.nodesCount, false) {}

void Dijkstra::run() {
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
            for (size_t arcId : this->G.outgoingArcs(minLabel.n)) {
            //for (size_t arcId : this->G.outgoingArcs(minLabel.n)) {
                if (this->isActive(arcId)) {
                    const Arc &a{this->G.arc(arcId)};
                    if (!this->permanent[a.targetId]) { //No need to propagate if a smaller label has been set.
                        this->insert(a.targetId, minLabel.c + this->arcCosts[arcId], a.sourceId, arcId);
                    }
                }
            }
        }
    }
}

std::vector<const Arc*> Dijkstra::getArcsToNode(std::size_t n) const {
    assert(this->permanent[n]); //If not, the node was not reached during the search or no search was yet performed.
    std::vector<const Arc *> path;
    const Label *l{&this->result[n]};
    while (l->predArc != INVALID) {
        path.push_back(&this->G.arc(l->predArc));
        l = &this->result[l->predNode];
    }
    std::reverse(path.begin(), path.end());
    return path;
}


Path Dijkstra::getPathToNode(Node n) const {
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
    std::reverse(path.begin(), path.end());
    return path;
}

int Dijkstra::distance(std::size_t n) const {
    return this->result[n].c;
}

Label Dijkstra::extractMin() {
    const Label l{this->H.top()};
    this->H.pop();

    //const Label l{*this->H.begin()};
    //this->H.erase(this->H.begin());
    return l;
}

void Dijkstra::makePermanent(const Label& l) {
    this->permanent[l.n] = true;
    this->result[l.n] = l;
}

void Dijkstra::printDistances() const {
    printf("SourceId: %lu\n", this->sourceId);
    for (size_t n = 0; n < this->G.nodesCount; ++n) {
        printf("(%lu, %d)\n", n, this->cost(n).c);
    }
}
