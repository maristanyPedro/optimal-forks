#ifndef LABEL_H_
#define LABEL_H_

#include "../../definitions/includes/constants.h"

typedef int CostType;

struct Label{
    Label():
        n{INVALID}, c{MAX_COST}, predNode{INVALID}, predArc{INVALID} {}

    Label(std::size_t node, CostType costs, std::size_t predNode, std::size_t predArc):
        n{node}, c{costs}, predNode{predNode}, predArc{predArc} {}

    void print() const;

    friend bool operator<(const Label& lhs, const Label& rhs) {
        if (lhs.c == rhs.c) {
            return lhs.n > rhs.n;
        }
        return lhs.c > rhs.c;
    }

    std::size_t n;
    CostType c = MAX_COST;
    std::size_t predNode;
    std::size_t predArc;
};

#endif
