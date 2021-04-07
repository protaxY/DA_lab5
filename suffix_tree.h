#pragma once

#include <vector>
#include <string>
#include <unordered_map>

int* END = new int(-1);

struct node {
    int l;
    int* r = END;
    std::unordered_map<char, node*> next;
    node* suffixLink = nullptr;
    ~node(){
        for (auto& i: next){
            delete i.second;
        }
        if (r != END){
            delete r;
        }
    }
    node* backNode = nullptr;
};

std::string pattern;
node* activeNode = nullptr;
node* activeEdge = nullptr;
int activeLength = 0;
int remainder = 0;
node* root = new node();
node* previousNode = root;

void Update(int pos){
    ++(*END);
    ++remainder;
    if (activeEdge == nullptr){
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
        }
    } else if (pattern[activeEdge->l + activeLength] == pattern[pos]){
        ++activeLength;
        if (activeLength == *activeEdge->r - activeEdge->l + 1){
            activeNode = activeEdge;
            activeEdge = nullptr;
            activeLength = 0;
        }
        return;
    }

    while (remainder){
        if (activeLength != 0){
            node* newNode = new node();
            newNode->suffixLink = root;
            newNode->l = pos;

            node* sideNode = new node();
            sideNode->l = activeEdge->l;
            sideNode->r = new int(sideNode->l + activeLength - 1);
            sideNode->suffixLink = root;
            sideNode->backNode = activeNode;

            activeNode->next[pattern[activeEdge->l]] = sideNode;

            activeEdge->l = *sideNode->r + 1;
            activeEdge->backNode = sideNode;

            sideNode->next.insert({pattern[newNode->l], newNode});
            sideNode->next.insert({pattern[activeEdge->l], activeEdge});

            newNode->backNode = sideNode;

            activeEdge = sideNode;
            if (previousNode != root ){
                previousNode->suffixLink = activeEdge;
            }
        } else {
            auto next = activeNode->next.find(pattern[pos]);
            if (next == activeNode->next.end()){
                node* newNode = new node();
                newNode->suffixLink = root;
                newNode->l = pos;
                newNode->backNode = activeNode;
                activeNode->next.insert({pattern[newNode->l], newNode});
            } else {
                activeEdge = next->second;
                ++activeLength;
                if (activeLength == *activeEdge->r - activeEdge->l + 1) {
                    activeNode = activeEdge;
                    activeLength = 0;
                    activeEdge = nullptr;
                }
                previousNode = root;
                return;
            }
        }

        --remainder;

        if (remainder) {
            if (activeLength != 0) {
                previousNode = activeEdge;
            } else {
                previousNode = root;
            }
            while (activeNode->suffixLink == root && activeNode != root){
                activeLength += *activeNode->r - activeNode->l + 1;
                activeNode = activeNode->backNode;
            }
            activeNode = activeNode->suffixLink;

            int cnt;
            if (activeNode == root){
                cnt = remainder - 1;
            } else {
                cnt = activeLength;
            }
            activeEdge = nullptr;
            activeLength = 0;

            while (cnt > 0) {
                auto next = activeNode->next.find(pattern[pos - cnt]);
                activeEdge = next->second;
                if (*activeEdge->r - activeEdge->l + 1 <= cnt){
                    cnt -= *activeEdge->r - activeEdge->l + 1;
                    activeNode = activeEdge;
                    activeLength = 0;
                    activeEdge = nullptr;
                } else {
                    activeLength = cnt;
                    cnt = 0;
                }
            }
        }
    }

    activeEdge = nullptr;
    activeLength = 0;
    activeNode = root;
    previousNode = root;
}

void print(node* cur, int height) {
    if (cur != nullptr) {
        for (int i = 0; i < height; ++i) {
            std::cout << "   ";
        }
        for (int i = cur->l; i <= *cur->r; ++i) {
            std::cout << pattern[i];
        }
        std::cout << std::endl;
        for (std::unordered_map<char, node*>::iterator it = cur->next.begin(); it != cur->next.end(); ++it) {
            print(it->second, height + 1);
        }
    }
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