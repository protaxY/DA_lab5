#include <iostream>
#include <vector>
#include "suffix_tree.h"

void removeFirst(int pos){
    if (activeNode == root){
        --activeLength;
    }

    activeNode = activeNode->suffixLink;

    auto next = activeNode->next.find(pattern[pos - reminder]);
    if (next != activeNode->next.end()){
        activeEdge = next->second;
    }
    int cnt = activeLength;

    if (cnt <= 0){
        activeEdge = nullptr;
    }
    activeLength = 0;
    int a = cnt;
    while(a > 0 /*pattern[activeEdge->l + activeLength] == pattern[pos - cnt + activeLength]*/){
        if (activeEdge != nullptr){
            ++activeLength;
            if (activeLength == *activeEdge->r - activeEdge->l + 1){
                activeNode = activeEdge;
                activeLength = 0;
                activeEdge = nullptr;
            }
        } else {
            auto next = activeNode->next.find(pattern[pos - activeLength - cnt + a]);
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


int main(){
    std::cin >> pattern;
    pattern += '$';
    Build();

//    std::string text;
//    std::cin >> text;
//    text += '$';
//    std::vector<int> ms(text.length());
//
//    activeNode = root;
//    activeLength = 0;
//    activeEdge = nullptr;
//    reminder = 0;
//    for (int i = 0; i < ms.size(); ++i){
//        if (activeEdge == nullptr){
//            auto next = activeNode->next.find(text[i]);
//            if (next != activeNode->next.end()){
//                ++reminder;
//                ++activeLength;
//                activeEdge = next->second;
//                if (activeLength == *activeEdge->r - activeEdge->l + 1){
//                    activeNode = activeEdge;
//                    activeEdge = nullptr;
//                    activeLength = 0;
//                }
//            } else {
//                ms[i - reminder] = reminder;
//                if (reminder > 0){
////                    --activeLength;
//                    --reminder;
//                    removeFirst(reminder);
//                    --i;
//                    if (activeNode == root && activeEdge == nullptr){
//                        reminder = 0;
//                    }
//                }
//            }
//        } else if (pattern[activeEdge->l + activeLength] == text[i]){
//            ++reminder;
//            ++activeLength;
//            if (activeLength == *activeEdge->r - activeEdge->l + 1){
//                activeNode = activeEdge;
//                activeEdge = nullptr;
//                activeLength = 0;
//            }
//        } else {
//            ms[i - reminder] = reminder;
//            if (reminder > 0){
////                --activeLength;
//                --reminder;
//                removeFirst(reminder);
//                --i;
//                if (activeNode == root && activeEdge == nullptr){
//                    reminder = 0;
//                }
//            }
//        }
//    }
//    while (reminder){
//        ms[ms.size() - reminder] = reminder - 1;
//        --reminder;
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

    return 0;
}

