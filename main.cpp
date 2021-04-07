#include <iostream>
#include <vector>
#include "suffix_tree.h"

std::string text;

void removeFirst(int pos){
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
        auto next = activeNode->next.find(text[pos - cnt]);
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

int main(){
    std::cin >> pattern;
    pattern += '$';
    Build();

    std::cin >> text;
    text += '$';
    std::vector<int> ms(text.length());

    activeNode = root;
    activeLength = 0;
    activeEdge = nullptr;
    remainder = 0;

    for (int i = 0; i < text.length(); ++i){
        ++remainder;
        if (activeEdge == nullptr){
            auto next = activeNode->next.find(text[i]);
            if (next != activeNode->next.end()){
                ++activeLength;
                activeEdge = next->second;
                if (activeLength == *activeEdge->r - activeEdge->l + 1){
                    activeNode = activeEdge;
                    activeEdge = nullptr;
                    activeLength = 0;
                }
            } else {
                --remainder;
                ms[i - remainder] = remainder;
                if (remainder == pattern.length() - 1){
                    std::cout << i - remainder + 1 << '\n';
                }
                removeFirst(i);
                if (remainder){
                    --remainder;
                    --i;
                }
            }
        } else if (pattern[activeEdge->l + activeLength] == text[i]){
            ++activeLength;
            if (activeLength == *activeEdge->r - activeEdge->l + 1){
                activeNode = activeEdge;
                activeEdge = nullptr;
                activeLength = 0;
            }
        } else {
            --remainder;
            ms[i - remainder] = remainder;
            if (remainder == pattern.length() - 1){
                std::cout << i - remainder + 1 << '\n';
            }
            removeFirst(i);
            if (remainder){
                --remainder;
                --i;
            }
        }
    }

    while (remainder){
        ms[ms.size() - remainder] = remainder - 1;
        if (remainder - 1 == pattern.length() - 1){
            std::cout << ms.size() - remainder + 1 << '\n';
        }
        --remainder;
    }

    delete root;
    delete END;

    return 0;
}
