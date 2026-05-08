#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include "Condition.h"
#include "Beverage.h"

class Rule {
private:
    std::string name;
    std::vector<Condition> conditions;
    Beverage conclusion;

public:
   Rule(std::string name, std::initializer_list<Condition> conditions, Beverage conclusion);

    const std::string& getName() const;
    const std::vector<Condition>& getConditions() const;
    Beverage getConclusion() const;
};

#endif