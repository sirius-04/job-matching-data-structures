#ifndef SKILL_WEIGHT_HPP
#define SKILL_WEIGHT_HPP

#include <iostream>
#include <string>
using namespace std;

struct SkillWeight {
    string skillName;
    int weight;
};

class SkillWeightList {
private:
    SkillWeight* data;
    int size;
    int capacity;

public:
  SkillWeightList(int cap = 20);
  ~SkillWeightList();

  void add(const string& skill, int weight);
  int getWeight(const string& skill) const;
};

#endif