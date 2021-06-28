#include <cstdio> //For fprint.

#include "../includes/Label.h"

using namespace std;

void Label::print() const {
    printf("%lu;%d;%lu\n", this->n, this->c, this->predNode);
}