
#include <chrono>  // for high_resolution_clock
#include <iostream>

#include "../../graph/includes/graph.h"

#include "../includes/forks.h"

#include "../includes/WeakArcFinder.h"

using namespace std;

WeakArcFinder::WeakArcFinder(const Graph &G):
    G{G},
    upperBounds(this->G, this->G.getUpperBoundCosts()) {

    auto startA2A = std::chrono::high_resolution_clock::now();
    this->upperBounds.run();
    auto finishA2A = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = finishA2A - startA2A;
    std::cout << "Twice all to all took: " << duration.count() << " s\n";
}

std::vector<const Arc*> WeakArcFinder::collectWeakArcs(const Node sourceId) const {
    auto startWeakArcs = std::chrono::high_resolution_clock::now();
    std::vector<const Arc*> weakArcs;
    ForkAlgorithm forkCalculator(this->G, sourceId, this->upperBounds);
    for (size_t arcId = 0; arcId < this->G.arcsCount; ++arcId) {
        const Arc &a{this->G.arc(arcId)};
        if (this->upperBounds.distance(sourceId, a.sourceId) == MAX_COST ||
            this->upperBounds.distance(sourceId, a.targetId) == MAX_COST) {
            continue;
        }
        auto startLokal = std::chrono::high_resolution_clock::now();
        int forkValue = forkCalculator.run(a.targetId, a.sourceId);
        auto endLokal = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> lokalDuration = endLokal - startLokal;
        bool isWeak = forkValue >= a.lowerBound;
            //printf("fork;%lu;", this->G.getOriginalId(sourceId));
            //this->G.printOriginalArc(arcId);
            //printf("%d;%d;%f;%d\n", forkValue, a.lowerBound, lokalDuration.count(), isWeak);
            if (isWeak) {
                    weakArcs.push_back(&a);
            }
    }
    auto endWeakArcs = std::chrono::high_resolution_clock::now();
    printf("result;%s;%lu;%lu;%lu;",
           this->G.getName().c_str(), this->G.getOriginalId(sourceId), weakArcs.size(), this->G.arcsCount);
    std::chrono::duration<double> duration = endWeakArcs - startWeakArcs;
    printf("%f\n", duration.count());
    return weakArcs;
}
