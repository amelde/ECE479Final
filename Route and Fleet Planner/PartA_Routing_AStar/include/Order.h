#ifndef ORDER_H
#define ORDER_H

#include <string>

struct Order {
    int id = 0;
    std::string destination;
    int loadUnits = 0;
    bool assigned = false;
};

#endif
