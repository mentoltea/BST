#include "BST.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

int main() {
    srand(time(nullptr));

    BinarySearchTree tree;
    BinarySearchTree tree2;
    for (int i=15; i>1; i--) {
        tree.insert(rand()%i, rand()%i);
    }
    tree.output_tree();
    cout << "--------" << endl;
    
    auto range = tree.equalRange(tree.begin()->first);
    cout << tree.begin()->first << endl;
    auto it = range.first;
    for (; it != range.second; it++)  {
        cout << "{" << it->first << ", " << it->second << "}" << endl;
    }
    try {
        it++;
        cout << "{" << it->first << ", " << it->second << "}" << endl;
    } catch (std::runtime_error e) {
        cout << "error " << e.what() << endl;
    }
}