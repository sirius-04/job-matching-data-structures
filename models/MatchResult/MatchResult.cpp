#include "MatchResult.hpp"

MatchResult::MatchResult() {}

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

    while (current != nullptr)
    {
        MatchResultNode* temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    length = 0;
}

void MatchResultList::printSlice() {
    int total = 0;

    MatchResultNode* p = head;
    while (p != NULL)
    {
        total++;
        p = p->next;
    }

    p = head;
    int index = 0;

    cout << "\n===== Match Result (Showing first 10 and last 10) =====\n";

    while (p != nullptr)
    {
        if (index < 5 || index >= total - 5)
        {
            cout << "|Job Id| " << p->data.jobId << " || " << "|Resume Id| " << p->data.resumeId << " || " << "|Score|" << p->data.score << endl;
        }
        else if (index == 10)
        {
            cout << "...\n...\n...(skipping " << total - 10 << " jobs)...\n...\n...\n";
        }

        p = p->next;
        index++;
    }

    cout << endl;
}

void MatchResultList::append(MatchResult data) {
    MatchResultNode* newNode = new MatchResultNode(data);

    if (length == 0)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    length++;
}

int MatchResultList::getLength() {
     return length;
}