#include <iostream>
#include "SPASystem.h"
#include "Beverage.h"

int main() {
    SPASystem system;

    Beverage recommendation = system.recommendBeverage();

    std::cout << "\nRecommended beverage: "
              << toString(recommendation)
              << '\n';

    return 0;
}