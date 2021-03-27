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

std::string str;
node* activeNode = nullptr;
node* activeEdge = nullptr;
int activeLength = 0;
int reminder = 0;
node* root = new node();
node* previousNode = root;

void Update(int pos){
    ++END;
    if (activeEdge == nullptr){
        auto next = activeNode->next.find(str[pos]);
        if (next != activeNode->next.end()){
            ++activeLength;
            ++reminder;
            activeEdge = next->second;
            if (activeLength == *activeEdge->r - activeEdge->l + 1){
                activeNode = activeEdge;
                activeEdge = nullptr;
                activeLength = 0;
            }
        } else {
            node* newNode = new node();
            newNode->l = pos;
            newNode->suffixLink = root;
            activeNode->next.insert({str[newNode->l], newNode});
        }
        return;
    }
    if (str[activeEdge->l + activeLength] == str[pos]){
        if (activeLength == *activeEdge->r - activeEdge->l + 1){
            activeNode = activeEdge;
            activeEdge = nullptr;
            activeLength = 0;
            return;
        }
        ++activeLength;
        ++reminder;
        return;
    } else {
        while (reminder){
            node* newNode = new node();
            newNode->suffixLink = root;
            newNode->l = pos;
            if (activeLength != 0){
                node* sideNode = new node();
                sideNode->suffixLink = root;
                sideNode->l = activeEdge->l + activeLength;
                sideNode->r = activeEdge->r;
                sideNode->next = activeEdge->next;
                activeEdge->next.clear(); // может не надо чистить, хз
                activeEdge->r = new int(activeEdge->l + activeLength - 1);
                activeEdge->next.insert({str[sideNode->l], sideNode});
                if (previousNode != root){
                    previousNode = sideNode; // ЧТО??
                }
                activeEdge->next.insert({str[newNode->l], newNode});

                if (previousNode != activeEdge && previousNode->suffixLink == root && previousNode != root){
                    previousNode->suffixLink = activeEdge;
                }
            } else {
                activeNode->next.insert({str[newNode->l], newNode});
            }

            --reminder;

            previousNode = activeEdge;

            if (reminder){
                activeNode = activeNode->suffixLink;
                auto next = activeNode->next.find(str[pos - reminder]);
                activeEdge = next->second;
                int cnt;
                if (*activeNode->r == -1){
                    cnt = activeLength - 1;
                } else {
                    cnt = activeLength;
                }
                if (cnt == 0){
                    activeEdge == nullptr;
                }
                activeLength = 0;
                while(cnt > 0 /*str[activeEdge->l + activeLength] == str[pos - cnt + activeLength]*/){
                    if (activeEdge == nullptr){
                        ++activeLength;
                        if (activeLength == *activeEdge->r - activeEdge->l + 1){
                            activeNode = activeEdge;
                            activeLength = 0;
                            activeEdge = nullptr;
                        }
                    } else {
                        auto next = activeNode->next.find(str[pos - cnt + activeLength]);
                        if (next == activeNode->next.end()){
                            activeEdge = nullptr; // произойдет ли это вообще
                            break;
                        } else {
                            activeEdge = next->second;
                            ++activeLength;
                            if (activeLength == *activeEdge->r - activeEdge->l + 1){
                                activeNode = activeEdge;
                                activeLength = 0;
                                activeEdge = nullptr;
                            }
                        }
                    }
                    --cnt;
                }
            }
        }
        previousNode = root;
        activeEdge = nullptr;
        activeLength = 0;
        activeNode = root;
        --END;
        Update(pos);
    }
}

node* Build(){
    root->l = -1;
    root->r = new int(-1);
    root->suffixLink = root;
    activeNode = root;
    for (int i = 0; i < str.length(); ++i){
        Update(i);
    }
}