#include "BST.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

int main() {
    srand(time(nullptr));

    BinarySearchTree tree;
    tree.insert(0,0);
    for (int i=1; i<10; i++) {
        tree.insert(i, 2*i);
    }
    BinarySearchTree::Iterator It1 = tree.find(5);
    BinarySearchTree::Iterator It2 = tree.find(6);
    auto pair = make_pair(It1, It2);
    auto rng = tree.equalRange(5);
    cout << (pair == rng) << endl;
}