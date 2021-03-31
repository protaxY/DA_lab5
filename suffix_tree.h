#pragma once

#include <vector>
#include <string>
#include <unordered_map>

int END = -1;

struct node {
    int l;
    int* r = &END;
    std::unordered_map<char, node*> next;
    node* suffixLink = nullptr;
};

std::string pattern;
node* activeNode = nullptr;
node* activeEdge = nullptr;
int activeLength = 0;
int reminder = 0;
node* root = new node();
node* previousNode = root;

void Update(int pos){
    ++END;
    if (activeEdge == nullptr){
        ++reminder;
        auto next = activeNode->next.find(pattern[pos]);
        if (next != activeNode->next.end()){
            ++activeLength;
            activeEdge = next->second;
            if (activeLength == *activeEdge->r - activeEdge->l + 1){
                activeNode = activeEdge;
                activeEdge = nullptr;
                activeLength = 0;
            }
            return;
        } else {
            node* newNode = new node();
            newNode->l = pos;
            newNode->suffixLink = root;
            activeNode->next.insert({pattern[newNode->l], newNode});
            --reminder;
        }
    } else if (pattern[activeEdge->l + activeLength] == pattern[pos]){
        ++reminder;
        ++activeLength;
        if (activeLength == *activeEdge->r - activeEdge->l + 1){
            activeNode = activeEdge;
            activeEdge = nullptr;
            activeLength = 0;
            return;
        }
        return;
    }

    while (reminder){
        node* newNode = new node();
        newNode->suffixLink = root;
        newNode->l = pos;
        if (activeLength != 0){
            node* sideNode = new node();
            sideNode->l = activeEdge->l;
            sideNode->r = new int(sideNode->l + activeLength - 1);
            sideNode->suffixLink = root;

            activeNode->next.erase(pattern[activeEdge->l]);
            activeNode->next.insert({pattern[sideNode->l], sideNode});

            activeEdge->l = *sideNode->r + 1;

            sideNode->next.insert({pattern[newNode->l], newNode});
            sideNode->next.insert({pattern[activeEdge->l], activeEdge});

            activeEdge = sideNode;
            if (previousNode != root){
                previousNode->suffixLink = activeEdge;
            }
        } else {
            activeNode->next.insert({pattern[newNode->l], newNode});
            if (previousNode != root){
                previousNode->suffixLink = activeNode;
            }
        }

        --reminder;

        if (reminder) {
            if (activeLength != 0) {
                previousNode = activeEdge;
            } else {
                previousNode = root;
            }

            if (activeNode == root) {
                --activeLength;
            }

            activeNode = activeNode->suffixLink;

            int cnt;
            if (activeLength == 0) {
                activeEdge = nullptr;
            } else {
                std::unordered_map<char, node *>::iterator next = activeNode->next.find(pattern[pos - activeLength]);
                activeEdge = next->second;
            }
            activeEdge = nullptr;
            cnt = activeLength;
            activeLength = 0;
            while (cnt > 0) {
                if (activeEdge != nullptr) {
                    ++activeLength;
                    if (activeLength == *activeEdge->r - activeEdge->l + 1) {
                        activeNode = activeEdge;
                        activeLength = 0;
                        activeEdge = nullptr;
                    }
                } else {
                    auto next = activeNode->next.find(pattern[pos - cnt]);
                    if (next == activeNode->next.end()) {
                        activeEdge = nullptr; // произойдет ли это вообще
                        break;
                    } else {
                        activeEdge = next->second;
                        ++activeLength;
                        if (activeLength == *activeEdge->r - activeEdge->l + 1) {
                            activeNode = activeEdge;
                            activeLength = 0;
                            activeEdge = nullptr;
                        }
                    }
                }
                --cnt;
            }

            if (activeEdge != nullptr && pattern[activeEdge->l + activeLength] == pattern[pos]) {
                ++activeLength;
                ++reminder;
                return;
            }
        }
    }

    activeEdge = nullptr;
    activeLength = 0;
    activeNode = root;
    previousNode = root;
    --END;
    Update(pos);
}

void Build(){
    root->l = -1;
    root->r = new int(-1);
    root->suffixLink = root;
    activeNode = root;
    for (int i = 0; i < pattern.length(); ++i){
        Update(i);
    }
}