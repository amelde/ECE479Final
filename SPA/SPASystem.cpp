#include "SPASystem.h"
#include <iostream>

//Ruleset
const std::vector<Rule> SPASystem::rules = {
    Rule(
        "S1",
        {
            {Fact::HealthConscious, true},
            {Fact::CitrusAllergy, true}
        },
        Beverage::CarrotJuice
    ),
    Rule(
        "S2",
        {
            {Fact::HealthConscious, true},
            {Fact::CitrusAllergy, false}
        },
        Beverage::OrangeJuice
    ),
    Rule(
        "S3",
        {
            {Fact::MealChicken, true},
            {Fact::LikesWine, true}
        },
        Beverage::Chardonnay
    ),
    Rule(
        "S4",
        {
            {Fact::MealSteak, true},
            {Fact::LikesWine, true}
        },
        Beverage::CabernetSauvignon
    ),
    Rule(
        "S5",
        {
            {Fact::Casual, true},
            {Fact::LikesBeer, true}
        },
        Beverage::Corona
    ),
    Rule(
        "S6",
        {
            {Fact::SportsNight, true},
            {Fact::LikesBeer, true}
        },
        Beverage::DosEquis
    ),
    Rule(
        "S7",
        {
            {Fact::NewYearsEve, true},
            {Fact::LikesLiquor, true}
        },
        Beverage::PolishVodka
    ),
    Rule(
        "S8",
        {
            {Fact::WeatherHot, true},
            {Fact::DoesNotDrinkAlcohol, true}
        },
        Beverage::SparklingWater
    ),
    Rule(
        "S9",
        {
            {Fact::DoesNotDrinkAlcohol, true},
            {Fact::HealthConscious, false}
        },
        Beverage::AppleJuice
    ),
    Rule(
        "S10",
        {
            {Fact::WeatherHot, true},
            {Fact::Casual, true}
        },
        Beverage::IcedTea
    ),
    Rule(
        "S11",
        {
            {Fact::MealSpicy, true},
            {Fact::DoesNotDrinkAlcohol, true}
        },
        Beverage::MangoLassi
    ),
    Rule(
        "S12",
        {
            {Fact::MealDessert, true},
            {Fact::LikesCoffee, true}
        },
        Beverage::Espresso
    ),
    Rule(
        "S13",
        {
            {Fact::Formal, true},
            {Fact::LikesWine, true}
        },
        Beverage::Champagne
    ),
    Rule(
        "S14",
        {
            {Fact::Brunch, true},
            {Fact::LikesLiquor, true}
        },
        Beverage::Mimosa
    ),
    Rule(
        "S15",
        {
            {Fact::HealthConscious, true},
            {Fact::WeatherHot, true}
        },
        Beverage::CoconutWater
    ),
    Rule(
        "S16",
        {
            {Fact::MealFish, true},
            {Fact::LikesWine, true}
        },
        Beverage::SauvignonBlanc
    ),
    Rule(
        "S17",
        {
            {Fact::DoesNotDrinkAlcohol, true},
            {Fact::Formal, true}
        },
        Beverage::SparklingCider
    ),
    Rule(
        "S18",
        {
            {Fact::HealthConscious, true},
            {Fact::DoesNotDrinkAlcohol, true}
        },
        Beverage::HerbalTea
    )
};

Beverage SPASystem::recommendBeverage() {
    std::vector<Beverage> goals = {
    // Health / allergy-specific
    Beverage::CarrotJuice,
    Beverage::OrangeJuice,

    // Health / non-alcoholic
    Beverage::CoconutWater,
    Beverage::HerbalTea,

    // Meal + wine pairings
    Beverage::Chardonnay,
    Beverage::CabernetSauvignon,
    Beverage::SauvignonBlanc,

    // Event + wine/liquor
    Beverage::Champagne,
    Beverage::PolishVodka,
    Beverage::Mimosa,

    // Beer pairings
    Beverage::Corona,
    Beverage::DosEquis,
    Beverage::AmberAle,

    // Non-alcoholic / context-based
    Beverage::SparklingWater,
    Beverage::AppleJuice,
    Beverage::SparklingCider,
    Beverage::IcedTea,
    Beverage::MangoLassi,
    Beverage::Espresso
};

    for (Beverage goal : goals) {
        if (!proveGoal(goal)) continue;

        return goal;
    }

    return Beverage::BottledWater;
}

bool SPASystem::ruleConditionsAreTrue(const Rule& rule) {
    for (const Condition& condition : rule.getConditions()) {
        bool actualValue = proveFact(condition.fact);

        if (actualValue != condition.expectedValue) return false;
    }

    return true;
}

bool SPASystem::proveGoal(Beverage goal) {
    for (const Rule& rule : rules) {
        if (rule.getConclusion() != goal) continue;

        std::cout << "\nTrying " << rule.getName() << " to prove " << toString(goal) << '\n';
        if (ruleConditionsAreTrue(rule)) {
            std::cout << rule.getName() << " succeeded.\n";
            return true;
        }
        std::cout << rule.getName() << " failed.\n";
    }

    return false;
}

bool SPASystem::proveFact(Fact fact) {
    auto it = knownFacts.find(fact);
    if (it != knownFacts.end()) return it->second == TruthValue::True;

    bool ans = askUserAboutFact(fact);
    knownFacts[fact] = ans ? TruthValue::True : TruthValue::False;

    return ans;
}

bool SPASystem::askUserAboutFact(Fact fact) {
    char resp;

    std::cout << questionForFact(fact) << " (y/n): ";
    std::cin >> resp;

    return resp == 'y' || resp == 'Y';
}

std::string SPASystem::questionForFact(Fact fact) const {
    switch (fact) {
        // Health
        case Fact::HealthConscious:
            return "Is the guest health-conscious?";
        case Fact::CitrusAllergy:
            return "Does the guest have a citrus allergy?";

        // Preferences
        case Fact::LikesWine:
            return "Does the guest like wine?";
        case Fact::LikesBeer:
            return "Does the guest like beer?";
        case Fact::LikesLiquor:
            return "Does the guest like liquor?";
        case Fact::LikesCoffee:
            return "Does the guest like coffee?";
        case Fact::DoesNotDrinkAlcohol:
            return "Does the guest avoid alcohol?";

        // Meal
        case Fact::MealChicken:
            return "Is the meal chicken?";
        case Fact::MealSteak:
            return "Is the meal steak?";
        case Fact::MealFish:
            return "Is the meal fish?";
        case Fact::MealSpicy:
            return "Is the meal spicy?";
        case Fact::MealDessert:
            return "Is dessert being served?";
        case Fact::MealBarbecue:
            return "Is the meal barbecue?";

        // Weather
        case Fact::WeatherHot:
            return "Is the weather hot?";

        // Event
        case Fact::NewYearsEve:
            return "Is the occasion New Year's Eve?";
        case Fact::Formal:
            return "Is the event formal?";
        case Fact::Brunch:
            return "Is the event brunch?";
        case Fact::Casual:
            return "Is the event casual?";
        case Fact::SportsNight:
            return "Is this a sports night?";
    }

    return "Unknown question";
}