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
        ++activeLength;
        ++reminder;
        if (activeLength == *activeEdge->r - activeEdge->l + 1){
            activeNode = activeEdge;
            activeEdge = nullptr;
            activeLength = 0;
            return;
        }
        return;
    } else {
        while (reminder){
            node* newNode = new node();
            newNode->suffixLink = root;
            newNode->l = pos;
            if (activeLength != 0){
                node* sideNode = new node();
                sideNode->l = activeEdge->l;
                sideNode->r = new int(sideNode->l + activeLength - 1);
                sideNode->suffixLink = root;

                activeNode->next.erase(str[activeEdge->l]);
                activeNode->next.insert({str[sideNode->l], sideNode});

                activeEdge->l = *sideNode->r + 1;

                sideNode->next.insert({str[newNode->l], newNode});
                sideNode->next.insert({str[activeEdge->l], activeEdge});

                activeEdge = sideNode;
                if (previousNode != root){
                    previousNode->suffixLink = activeEdge;
                }
//                sideNode->suffixLink = activeEdge->suffixLink;
//                sideNode->l = activeEdge->l + activeLength;
//                sideNode->r = activeEdge->r;
//                sideNode->next = activeEdge->next;
//                activeEdge->next.clear(); // может не надо чистить, хз
//                activeEdge->suffixLink = root;
//                activeEdge->r = new int(activeEdge->l + activeLength - 1);
//                activeEdge->next.insert({str[sideNode->l], sideNode});
//                activeEdge->next.insert({str[newNode->l], newNode});
            } else {
                activeNode->next.insert({str[newNode->l], newNode});
            }

            --reminder;

            if (reminder){
                if (activeLength != 0){
                    previousNode = activeEdge;
                } else {
                    previousNode = root;
                }

                if (activeNode == root){
                    --activeLength;
                }

                activeNode = activeNode->suffixLink;
                auto next = activeNode->next.find(str[pos - activeLength]);
                activeEdge = next->second;
                int cnt = activeLength;
//                if (*activeNode->r == -1){
//                    cnt = activeLength - 1;
//                } else {
//                    cnt = activeLength;
//                }
                if (cnt == 0){
                    activeEdge == nullptr;
                }
                activeLength = 0;
                int a = cnt;
                while(a > 0 /*str[activeEdge->l + activeLength] == str[pos - cnt + activeLength]*/){
                    if (activeEdge != nullptr){
                        ++activeLength;
                        if (activeLength == *activeEdge->r - activeEdge->l + 1){
                            activeNode = activeEdge;
                            activeLength = 0;
                            activeEdge = nullptr;
                        }
                    } else {
                        auto next = activeNode->next.find(str[pos - activeLength + cnt - a]);
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
                    --a;
                }
            }
        }
        previousNode->suffixLink = activeEdge;
        activeEdge = nullptr;
        activeLength = 0;
        activeNode = root;
        previousNode = root;
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