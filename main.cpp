#include <iostream>
#include "SPA/SPASystem.h"
#include "SPA/Beverage.h"

int main() {
    SPASystem system;

    Beverage recommendation = system.recommendBeverage();

    std::cout << "\nRecommended beverage: "
              << toString(recommendation)
              << '\n';

    return 0;
}