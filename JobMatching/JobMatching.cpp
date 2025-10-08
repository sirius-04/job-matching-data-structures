#include "JobMatching.hpp"

MatchResult::MatchResult(int jobId, int resumeId, double score) {
    this->jobId = jobId;
    this->resumeId = resumeId;
    this->score = score;
}

MatchResultNode::MatchResultNode(MatchResult data) {
    this->data = data;
    this->next = nullptr;
}

MatchResultList::MatchResultList() {
    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;
}

MatchResultList::~MatchResultList() {
    MatchResultNode* current = head;

    while (current != nullptr) {
        MatchResultNode* temp = current;
        current = current->next;

        delete temp;
    }
}

void MatchResultList::printList() {
    MatchResultNode* temp = head;
    int count = 0;

    cout << "===== Match Results =====" << endl;
    while (temp != nullptr) {
        cout << "Job ID: " << temp->data.jobId
             << " | Resume ID: " << temp->data.resumeId
             << " | Score: " << temp->data.score << "%" << endl;

        temp = temp->next;
    }
}

void MatchResultList::append(int jobId, int resumeId, double score) {
    MatchResult newResult(jobId, resumeId, score);
    MatchResultNode* node = new MatchResultNode(newResult);

    if (!head) head = node;
    else {
        MatchResultNode* temp = head;

        while (temp->next !=nullptr) {
            temp = temp->next;
        }

        temp->next = node;
    }

    length++;
}

int MatchResultList::getLength() {
     return length;
}

JobMatching::JobMatching(JobLinkedList* jobs, ResumeLinkedList* resumes, SearchAlgorithm selectedAlgorithm) {
    this->jobs = jobs;
    this->resumes = resumes;
    this->results = new MatchResultList();
    this->selectedAlgorithm = selectedAlgorithm;
    this->matchTime = 0;
}

JobMatching::~JobMatching() {
    delete results;
    results = nullptr;
}

double JobMatching::calculateSkillMatch(JobNode* job, ResumeNode* resume) {
}