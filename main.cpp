#include <iostream>
#include <vector>
#include "suffix_tree.h"

std::string text;

void removeFirst(int pos){
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
        std::unordered_map<char, node *>::iterator next = activeNode->next.find(text[pos - activeLength]);
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
            auto next = activeNode->next.find(text[pos - cnt]);
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
}


std::string MinCycleString() {
    std::string res;
    res.reserve(pattern.size());
    size_t size = pattern.size();

    pattern += pattern;
    pattern += '$';
    Build();
//    sf.print();

//    SuffTree::Node* iter = sf.root;
    node* iter = root;
    while (res.size() < size) {
        char key = iter->next.begin()->first;
        for (std::unordered_map<char, node*>::iterator it = ++iter->next.begin(); it != iter->next.end(); ++it) {
            if (it->first < key) {
                key = it->first;
            }
        }

        iter = iter->next[key];
        for (int i = iter->l; i <= *iter->r && res.size() < size; ++i) {
            res.push_back(pattern[i]);
        }
    }
    return res;
}

int main(){
    std::cin >> pattern;
    std::cout << MinCycleString() << '\n';

//    std::cin >> pattern;
//    pattern += '$';
//    Build();
//
//    std::cin >> text;
//    text += '$';
//    std::vector<int> ms(text.length());
//
//    activeNode = root;
//    activeLength = 0;
//    activeEdge = nullptr;
//    remainder = 0;
//    for (int i = 0; i < ms.size(); ++i){
//        if (activeEdge == nullptr){
//            auto next = activeNode->next.find(text[i]);
//            if (next != activeNode->next.end()){
//                ++remainder;
//                ++activeLength;
//                activeEdge = next->second;
//                if (activeLength == *activeEdge->r - activeEdge->l + 1){
//                    activeNode = activeEdge;
//                    activeEdge = nullptr;
//                    activeLength = 0;
//                }
//            } else {
//                ms[i - remainder] = remainder;
//                if (remainder > 0){
//                    --remainder;
//                    if (activeNode == root && activeEdge == nullptr){
//                        continue;
//                    }
//                    removeFirst(i);
//                    --i;
//                }
//            }
//        } else if (pattern[activeEdge->l + activeLength] == text[i]){
//            ++remainder;
//            ++activeLength;
//            if (activeLength == *activeEdge->r - activeEdge->l + 1){
//                activeNode = activeEdge;
//                activeEdge = nullptr;
//                activeLength = 0;
//            }
//        } else {
//            ms[i - remainder] = remainder;
//            if (remainder > 0){
//                --remainder;
//                if (activeNode == root && activeEdge == nullptr){
//                    continue;
//                }
//                removeFirst(i);
//                --i;
//            }
//        }
//    }
//    while (remainder){
//        ms[ms.size() - remainder] = remainder - 1;
//        --remainder;
//    }
//
//    std::vector<int> ans;
//    for (int i = 0; i < ms.size() - 1; ++i){
//        if (ms[i] == pattern.length() - 1){
//            ans.push_back(i);
//        }
//    }
//    for (int i = 0; i < ans.size(); ++i){
//        std::cout << ans[i] + 1 << '\n';
//    }
//
//    delete root;
//    delete END;

    return 0;
}
