#pragma once

#include <vector>
#include <string>

int END = -1;

struct node {
    int l;
    int* r = &END;
    std::vector<node*> next;
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
    int nextEdge = -1;
    if (activeEdge == nullptr){
        for (int i = 0; i < activeNode->next.size(); ++i){
            if (str[activeNode->next[i]->l + activeLength] == str[pos]){
                nextEdge = i;
                break;
            }
        }
        if (nextEdge != -1){
            ++activeLength;
            ++reminder;
            activeEdge = activeNode->next[nextEdge];
            activeEdge->suffixLink = root;
        } else {
            node* newNode = new node();
            newNode->l = pos;
            activeNode->next.push_back(newNode);
            return;
//            activeEdge = nullptr;
        }
    } else if (str[activeEdge->l + activeLength] == str[pos]){
        ++activeLength;
        ++reminder;
        if (activeLength == *activeEdge->r - activeEdge->l + 1){
            activeNode = activeEdge;
            activeEdge = nullptr;
        }
        return;
    } else {
        while (reminder){
            node* newNode = new node();
            newNode->suffixLink = root;
            if (activeLength == 0){
                newNode->l = pos;
            } else {
                activeEdge->r = new int(activeEdge->l + activeLength - 1);
                newNode->l = pos;
                node* sideNode = new node();
                sideNode->suffixLink = root;
                sideNode->l = *activeEdge->r + 1;
                activeEdge->next.push_back(sideNode);
            }
            activeEdge->next.push_back(newNode);
            --reminder;
            if (reminder){
                activeLength = 0;
                activeNode = activeEdge->suffixLink;
                previousNode = activeEdge;
                for (int i = 0; i < activeNode->next.size(); ++i){
                    if (str[activeNode->next[i]->l + activeLength] == str[pos - reminder]){
                        nextEdge = i;
                        break;
                    }
                }
                activeEdge = activeNode->next[nextEdge];
                while(str[activeEdge->l + activeLength] == str[pos - reminder]){
                    ++activeLength;
                }
                previousNode->suffixLink = activeEdge;
            } else {
                previousNode->suffixLink = activeEdge;
                activeEdge = nullptr;
                activeLength = 0;
            }
        }
    }
}

node* Build(){
    root->l = -1;
    root->r = new int(-1);
    root->suffixLink = root;
    activeNode = root;
    for (int i = 0; i < str.length(); ++i){
        ++END;
        Update(i);
    }
}