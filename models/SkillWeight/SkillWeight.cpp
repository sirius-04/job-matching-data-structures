#include "SkillWeight.hpp"

SkillWeightList::SkillWeightList(int cap = 20) {
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
        if (data[i].skillName == skill)
            return data[i].weight;
    }
    return 0;
}
