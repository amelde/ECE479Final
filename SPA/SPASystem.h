#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include <vector>
#include <unordered_map>
#include "Rule.h"
#include "Fact.h"
#include "Beverage.h"

enum class TruthValue {
    Unknown,
    True,
    False
};

class SPASystem {
private:
    std::unordered_map<Fact, TruthValue> knownFacts;
    static const std::vector<Rule> rules;

    bool proveGoal(Beverage goal);
    bool ruleConditionsAreTrue(const Rule& rule);
    bool proveFact(Fact fact);
    bool askUserAboutFact(Fact fact);

    std::string questionForFact(Fact fact) const;

public:
    Beverage recommendBeverage();
};

#endif