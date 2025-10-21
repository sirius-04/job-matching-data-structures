#include "MatchResult.hpp"
#include <iomanip>
#include <algorithm>

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

    if (!head || length == 0) {
        cout << "\n No match results to display.\n";
        return;
    }
    
    // ===== Convert linked list to array for efficient sorting =====
    MatchResult* arr = new MatchResult[length];
    MatchResultNode* current = head;
    int idx = 0;
    
    while (current != nullptr && idx < length) {
        arr[idx++] = current->data;
        current = current->next;
    }
    
    // ===== Use std::sort (O(n log n)) instead of bubble sort =====
    sort(arr, arr + length, [](const MatchResult& a, const MatchResult& b) {
        return a.score > b.score;  // Descending order
    });

    // ===== Print top and bottom 10 =====
    cout << "\n===== Match Result (Descending by Score) =====\n";
    cout << "Total matches: " << length << "\n";
    
    if (length <= 20) {
        cout << "(Showing all results)\n\n";
        for (int i = 0; i < length; i++) {
            cout << "| Job ID: " << arr[i].jobId
                 << " | Resume ID: " << arr[i].resumeId
                 << " | Score: " << fixed << setprecision(2) << arr[i].score << " |\n";
        }
    } else {
        cout << "(Showing first 10 and last 10)\n\n";
        
        // Top 10
        for (int i = 0; i < 10; i++) {
            cout << "| Job ID: " << arr[i].jobId
                 << " | Resume ID: " << arr[i].resumeId
                 << " | Score: " << fixed << setprecision(2) << arr[i].score << " |\n";
        }
        
        cout << "...\n...(skipping " << (length - 20) << " matches)...\n...\n";
        
        // Bottom 10
        for (int i = length - 10; i < length; i++) {
            cout << "| Job ID: " << arr[i].jobId
                 << " | Resume ID: " << arr[i].resumeId
                 << " | Score: " << fixed << setprecision(2) << arr[i].score << " |\n";
        }
    }

    cout << "\nTotal matches displayed: " << min(20, length) << " of " << length << endl;
    
    delete[] arr;
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