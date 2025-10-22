#include "SkillWeight.hpp"

SkillWeightList::SkillWeightList(int cap) {
    capacity = cap;
    size = 0;
    data = new SkillWeight[capacity];
}

SkillWeightList::~SkillWeightList() {
    delete[] data;
}

void SkillWeightList::add(const string& skill, int weight) {
    if (size < capacity) {
        data[size++] = { skill, weight };
    }
}

int SkillWeightList::getWeight(const string& skill) const {
    for (int i = 0; i < size; i++) {
        string trimmedSkill = skill;
        trimmedSkill.erase(0, trimmedSkill.find_first_not_of(" \t\n\r"));
        trimmedSkill.erase(trimmedSkill.find_last_not_of(" \t\n\r") + 1);
        
        if (data[i].skillName == trimmedSkill)
            return data[i].weight;
    }
    return 0;
}
