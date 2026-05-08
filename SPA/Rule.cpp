#include "Rule.h"
#include <utility>

Rule::Rule(std::string name, std::initializer_list<Condition> conditions, Beverage conclusion)
    : name(std::move(name)), conditions(conditions), conclusion(conclusion) {}

const std::string& Rule::getName() const {
    return name;
}

const std::vector<Condition>& Rule::getConditions() const {
    return conditions;
}

Beverage Rule::getConclusion() const {
    return conclusion;
}