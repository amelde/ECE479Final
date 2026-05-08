#ifndef CONDITION_H
#define CONDITION_H

#include "Fact.h"

struct Condition {
    Fact fact;
    bool expectedValue;
};

#endif