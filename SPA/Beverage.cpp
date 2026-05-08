#include "Beverage.h"

std::string toString(Beverage beverage) {
    switch (beverage) {
        case Beverage::CarrotJuice:
            return "carrot juice";
        case Beverage::OrangeJuice:
            return "orange juice";
        case Beverage::Chardonnay:
            return "Chardonnay";
        case Beverage::CabernetSauvignon:
            return "Cabernet Sauvignon";
        case Beverage::Corona:
            return "Corona";
        case Beverage::DosEquis:
            return "Dos Equis";
        case Beverage::PolishVodka:
            return "Polish vodka";
        case Beverage::SparklingWater:
            return "sparkling water";
        case Beverage::AppleJuice:
            return "apple juice";
        case Beverage::BottledWater:
            return "bottled water";

        case Beverage::IcedTea:
            return "iced tea";
        case Beverage::MangoLassi:
            return "mango lassi";
        case Beverage::Espresso:
            return "espresso";
        case Beverage::SauvignonBlanc:
            return "Sauvignon Blanc";
        case Beverage::CoconutWater:
            return "coconut water";
        case Beverage::Champagne:
            return "Champagne";
        case Beverage::Mimosa:
            return "mimosa";
        case Beverage::SparklingCider:
            return "sparkling cider";
        case Beverage::AmberAle:
            return "amber ale";
        case Beverage::HerbalTea:
            return "herbal tea";
    }

    return "unknown beverage";
}