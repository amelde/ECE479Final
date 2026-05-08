#ifndef BEVERAGE_H
#define BEVERAGE_H

#include <string>

enum class Beverage {
    CarrotJuice,
    OrangeJuice,
    Chardonnay,
    CabernetSauvignon,
    Corona,
    DosEquis,
    PolishVodka,
    SparklingWater,
    AppleJuice,
    BottledWater,

    IcedTea,
    MangoLassi,
    Espresso,
    SauvignonBlanc,
    CoconutWater,
    Champagne,
    Mimosa,
    SparklingCider,
    AmberAle,
    HerbalTea
};

std::string toString(Beverage beverage);

#endif