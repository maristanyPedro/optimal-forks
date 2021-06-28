#include <cassert>
#include <fstream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "../includes/graph.h"

using namespace std;

std::pair<Node, Node> forwardArc(const Arc& a) {
    return {a.sourceId, a.targetId};
}
std::pair<Node, Node> backwardArc(const Arc& a) {
    return {a.targetId, a.sourceId};
}

void Graph::readGraph(const string& filename) {
    ifstream infile(filename);
    string line;
    size_t readLines = 0;
    assert(this->arcsCount == 0 && this->nodesCount == 0);
    while (getline(infile, line)) {
        ++readLines;
        vector<string> splittedLine{split(line, ';')};
        if (splittedLine[0] != "source" && splittedLine[0] != "#source") {
            assert(splittedLine.size() == 5);
            size_t tailId;
            std::stringstream(splittedLine[0]) >> tailId;
            size_t headId;
            std::stringstream(splittedLine[1]) >> headId;
            if (!this->exists(tailId)) {
                this->newIds.emplace(tailId, this->nodesCount);
                this->nodes.emplace_back(NodeAdjacency());
                this->nodes[this->nodesCount].id = tailId;
                if (stoi(splittedLine[4]) == 1) {
                    this->originNodes.push_back(this->nodesCount);
                }
                this->originalIds.push_back(tailId);
                ++this->nodesCount;
            }
            if (!this->exists(headId)) {
                this->newIds.emplace(headId, this->nodesCount);
                this->nodes.emplace_back(NodeAdjacency());
                this->nodes[this->nodesCount].id = headId;
                this->originalIds.push_back(headId);
                ++this->nodesCount;
            }
            size_t newTailId = this->newIds[tailId];
            size_t newHeadId = this->newIds[headId];
            this->nodes[newTailId].outgoingArcIds.push_back(this->arcsCount);
            this->nodes[newHeadId].incomingArcIds.push_back(this->arcsCount);
            const int lb = stoi(splittedLine[2]);
            const int ub = stoi(splittedLine[3]);
            this->arcs.emplace_back(this->arcsCount, newTailId, newHeadId, lb, ub);
            ++this->arcsCount;
        }
    }
    infile.close();
    assert(this->nodes.size() == this->newIds.size());
    assert(this->nodes.size() == this->nodesCount);
    std::sort(this->originNodes.begin(), this->originNodes.end());
    printf("$n= %lu \\ m=%lu$\n", this->nodesCount, this->arcsCount);
}

const std::vector<std::size_t>& Graph:: outgoingArcs(const Node nodeId) const {
    return this->nodes[nodeId].outgoingArcIds;
}

const std::vector<std::size_t>& Graph::incomingArcs(const Node nodeId) const {
    return this->nodes[nodeId].incomingArcIds;
}

void Graph::disableNode(const Node n, SubGraph& sg) const {
    for (size_t arcId: this->outgoingArcs(n)) {
        sg[arcId] = false;
    }
    for (size_t arcId: this->incomingArcs(n)) {
        sg[arcId] = false;
    }
}

void Graph::printNodeInfo(const Node nodeId) const {
    printf("Analyzing node: %lu\n", nodeId);
    printf("OUTGOING ARCS\n");
    printArcs(this->outgoingArcs(nodeId));
    printf("INCOMING ARCS\n");
    printArcs(this->incomingArcs(nodeId));
}

void Graph::printArcs(const vector<size_t>& arcIds) const {
    for (size_t arcId : arcIds) {
        //const Arc& arc{this->arcs[arcId]};
        //arc.print();
        this->printOriginalArc(arcId);
    }
}

void Graph::printOriginalArc(const size_t arcId) const {
    const Arc& a{this->arcs[arcId]};
    printf("%lu;%lu;%d;%d;", this->nodes[a.sourceId].id, this->nodes[a.targetId].id, a.lowerBound, a.upperBound);
}

void Graph::printSubGraph(const std::vector<const Arc*> sg, const Node sourceNode) const {
    printf("START_GRAPH;%lu\n", this->getOriginalId(sourceNode));
    printf("source;target;lower;upper;od\n");
    for(auto a : sg) {
        this->printOriginalArc(a->arcId);
        if (std::find(this->originNodes.begin(), this->originNodes.end(), a->sourceId) != this->originNodes.end()) {
            printf("1\n");
        }
        else if (std::find(this->originNodes.begin(), this->originNodes.end(), a->targetId) != this->originNodes.end()) {
            printf("-1\n");
        }
        else {
            printf("0\n");
        }
    }
    printf("END_GRAPH;%lu\n", this->getOriginalId(sourceNode));
}

const std::vector<std::size_t>& forwardStar(const Graph& G, const std::size_t nodeId) {
    return G.outgoingArcs(nodeId);
}
const std::vector<std::size_t>& backwardStar(const Graph& G, const std::size_t nodeId) {
    return G.incomingArcs(nodeId);
}

Arc::Arc(const size_t arcId, const Node sourceId, const Node targetId, const int lb, const int ub):
    arcId{arcId}, sourceId{sourceId}, targetId{targetId}, lowerBound{lb}, upperBound{ub} {}

Arc::Arc(): arcId{INVALID}, sourceId{INVALID}, targetId{INVALID}, lowerBound{MAX_COST}, upperBound{MAX_COST} {}

void Arc::print() const {
    printf("\t\t%lu -----(%d, %d)-----> %lu\n", sourceId, this->lowerBound, this->upperBound, targetId);
}

Graph::Graph(const std::string& filename):
    nodesCount{0},
    arcsCount{0},
    name{split(filename, '/').back()} {
    this->readGraph(filename);
    this->lowerBounds = this->buildCostVector([](const Arc& a){return a.lowerBound;});
    this->upperBounds = this->buildCostVector([](const Arc& a){return a.upperBound;});
}

std::vector<int> Graph::buildCostVector(int getKey(const Arc&)) const{
    std::vector<int> costs(this->arcsCount, MAX_COST);
    for (size_t arcId = 0; arcId < this->arcsCount; ++arcId) {
        const Arc& a{this->arc(arcId)};
        costs[arcId] = getKey(a);
    }
    return costs;
}

std::vector<int> buildCostScenario(const std::vector<int>& ubCosts, const std::vector<const Arc*>& lbArcs) {
    std::vector<int> costScenario(ubCosts);
    costScenario.shrink_to_fit();
    for (const Arc* a : lbArcs) {
        costScenario[a->arcId] = a->lowerBound;
    }
    return costScenario;
}

void split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> split(const string& s, char delim) {
    vector<string> elems;
    if (delim == ' ') {
        istringstream iss(s);
        elems = {istream_iterator<string>{iss}, istream_iterator<string>{}};
    } else {
        split(s, delim, elems);
    }

    for (auto& elem : elems) {
        if (elem.back() == '\n') {
            elem = elem.substr(0, elem.size() - 1);
        }
    }
    return elems;
}

NodeAdjacency::NodeAdjacency(): id{INVALID} {}

NodeAdjacency::NodeAdjacency(const std::size_t nid): id{nid} {}
