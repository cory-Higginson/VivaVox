#pragma once
#include "Cell.h"

#include <functional>
#include <vector>

struct ruleformat
{
  TYPE type1;
  TYPE type2;
  std::function<void(std::vector<Cube&>)> action;
};


class RuleTable
{
  std::vector<ruleformat> rules;
public:
  void addRules(ruleformat rule)
  {
    rules.emplace_back(rule);
  }
  void applyrules();
  void runrules();
};
