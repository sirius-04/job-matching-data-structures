#include "JobMatching.hpp"

JobMatching::JobMatching(JobLinkedList* jobs, ResumeLinkedList* resumes) {
    this->jobs = jobs;
    this->resumes = resumes;
    this->results = new MatchResultList();
    this->matchTime = 0;
}

JobMatching::~JobMatching() {
    delete results;
    results = nullptr;
}