#include "BST.h"

#include <iostream>


/*
BST NODE FUNCS
*/
BinarySearchTree::Node::Node(Key key,
                        Value value,
                        Node *parent,
                        Node *left,
                        Node *right) : keyValuePair(key, value), parent(parent),
                        left(left), right(right) {
}


BinarySearchTree::Node::Node(const BinarySearchTree::Node &other) {
    keyValuePair.first = other.keyValuePair.first;
    keyValuePair.second = other.keyValuePair.second;

    if (other.left) {
        left = copyTree(other.left);
        left->parent = this;
    }
    if (other.right) {
        right = copyTree(other.right);
        right->parent = this;
    }
}

BinarySearchTree::Node* BinarySearchTree::Node::copyTree(const Node *copy) {
    Node *newNode = new Node(copy->keyValuePair.first, copy->keyValuePair.second);
    if (copy->left) {
        newNode->left = copyTree(copy->left);
        newNode->left->parent = newNode;
    }
    if (copy->right) {
        newNode->right = copyTree(copy->right);
        newNode->right->parent = newNode;
    }
    
    return newNode;
}

void BinarySearchTree::Node::destroyTree() {
    if (left) left->destroyTree();
    if (right) right->destroyTree();
    delete this;
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    Key ourKey = keyValuePair.first;
    // добавить условия на left(right) != nullptr для рекурсивного добавления в поддеревья 
    if (key < ourKey) {
        if (left) {
            left->insert(key, value);
            return;
        }
        Node *newNode = new Node(key, value);
        left = newNode;
        newNode->parent = this;
    }
    else if (key >= ourKey) {
        if (right) {
            if (key == ourKey && right->keyValuePair.first != ourKey) {
                Node *newNode = new Node(key, value);
                newNode->parent = this;
                newNode->insertTree(right);
                right->destroyTree();
                right = newNode;
                return;
            }
            right->insert(key, value);
            return;
            
        }
        Node *newNode = new Node(key, value);
        right = newNode;
        newNode->parent = this;
    }
}

void BinarySearchTree::Node::insertTree(const Node* copy) {
    insert(copy->keyValuePair.first, copy->keyValuePair.second);
    if (copy->left) insertTree(copy->left);
    if (copy->right) insertTree(copy->right);
}


bool BinarySearchTree::Node::operator==(const Node& other) const {
    return (this->keyValuePair.first == other.keyValuePair.first &&
    this->parent == other.parent && this->left == other.left && this->right == other.right);
}

void BinarySearchTree::Node::output_node() const {
    std::cout << "{" << keyValuePair.first 
              << "," << keyValuePair.second << "}"
              << std::endl;
    if (left) {
        std::cout << "L ";
        left->output_node();
    }
    if (right) {
        std::cout << "R ";
        right->output_node();
    }
    std::cout << "UP ";
}

void BinarySearchTree::Node::erase(const Key &key) {
    Key ourKey = keyValuePair.first;
    if (ourKey != key) {
        if (key < ourKey && left) left->erase(key);
        else if (key > ourKey && right) right->erase(key);
        return;
    }

    // Заранее очищаем оставшуюся часть от возможных повторений
    if (right) right->erase(key);

    

    // Хотя бы одного потомка не существует
    if (!left || !right) {
        Node* child = (left) ? left : right;
        
        if (child == nullptr) {
            if (parent) {
                if (parent->left == this) parent->left = nullptr;
                else if (parent->right == this) parent->right = nullptr;
            }
            delete this;
            return;
        }

        keyValuePair.first = child->keyValuePair.first;
        keyValuePair.second = child->keyValuePair.second;

        left = child->left;
        right = child->right;
        
        if (child->left) child->left->parent = this;
        if (child->right) child->right->parent = this;

        delete child;
        return;
    }

    // Если существуют оба потомка

    Node* replacingChild = right;
    Node* movingChild = left;
    
    keyValuePair.first = replacingChild->keyValuePair.first;
    keyValuePair.second = replacingChild->keyValuePair.second;

    left = replacingChild->left;
    right = replacingChild->right;

    if (replacingChild->left) replacingChild->left->parent = this;
    if (replacingChild->right) replacingChild->right->parent = this;
    delete replacingChild;

    insertTree(movingChild);
    movingChild->destroyTree();
}

bool BinarySearchTree::Node::differsFrom(const Key &key) const {
    bool temp1 = false, temp2 = false;
    if (left) {temp1 = left->differsFrom(key);}
    if (right) {temp2 = right->differsFrom(key);}
    return temp1 || temp2 || (keyValuePair.first != key);
}

size_t BinarySearchTree::Node::size() const {
    size_t res = 1;
    if (left) res += left->size();
    if (right) res += right->size();
    return res;
}


/*
BST FUNCS
*/




BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) {
    if (other._root) _root = BinarySearchTree::Node::copyTree(other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (_root) _root->destroyTree();
    if (other._root) _root = BinarySearchTree::Node::copyTree(other._root);
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    _root = other._root;
    other._root = nullptr;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (_root) _root->destroyTree();
    _root = other._root;
    other._root = nullptr;
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    if (_root) _root->destroyTree();
}

void BinarySearchTree::destroyTree() {
    _root->destroyTree();
    _root = nullptr;
}


void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (_root) 
        _root->insert(key, value);
    else 
        _root = new Node(key, value);
}

BinarySearchTree::Node* BinarySearchTree::insert(const BinarySearchTree::Node &src) {
    _root->insertTree(&src);
    return _root;
}

void BinarySearchTree::erase(const Key &key) {
    if (_root->differsFrom(key)) {
        _root->erase(key);
    }
    else {
        _root->destroyTree();
        _root = nullptr;
    }
}

void BinarySearchTree::output_tree() {
    if (_root) {
        _root->output_node();
    }
    else {
        std::cout << "Empty" << std::endl;
    }
}


BinarySearchTree::Iterator::Iterator(BinarySearchTree::Node* node,
 BinarySearchTree::Node* root) {
    _node = node;
    _root = root;
}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (!_node) throw std::runtime_error("End iterator is nullptr");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (!_node) throw std::runtime_error("End iterator is nullptr");
    return _node->keyValuePair;
}


std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    if (!_node) throw std::runtime_error("End iterator is nullptr");
    return &(_node->keyValuePair);
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    if (!_node) throw std::runtime_error("End iterator is nullptr");
    return &(_node->keyValuePair);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (_node == nullptr) throw std::runtime_error("Iterator out of range (too big)");
    if (!_node->right) {
        if (!_node->parent) {_node = nullptr; return *this;}
        
        while (_node->parent->right == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) {_node = nullptr; return *this;}
        _node = _node->parent;
    }
    else {
        _node = _node->right;
        while (_node->left)
        {
            _node = _node->left;
        }
        
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    if (_node == nullptr) throw std::runtime_error("Iterator out of range (too big)");
    Node* oldnode = _node;
    if (!_node->right) {
        if (!_node->parent) {_node = nullptr; return Iterator(oldnode, _root);}
        
        while (_node->parent->right == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) {_node = nullptr; return Iterator(oldnode, _root);}
        _node = _node->parent;
    }
    else {
        _node = _node->right;
        while (_node->left)
        {
            _node = _node->left;
        }
        
    }
    return Iterator(oldnode, _root);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node == nullptr) {
        _node = _root;
        while (_node->right) {
            _node = _node->right;
        }
    }
    else if (!_node->left) {
        if (!_node->parent) throw std::runtime_error("Iterator out of range (too small)");
        
        while (_node->parent->left == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) throw std::runtime_error("Iterator out of range (too small)");
        _node = _node->parent;
    }
    else {
        _node = _node->left;
        while (_node->right)
        {
            _node = _node->right;
        }
        
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Node* oldnode = _node;
    if (_node == nullptr) {
        _node = _root;
        while (_node->right) {
            _node = _node->right;
        }
    }
    else if (!_node->left) {
        if (!_node->parent) throw std::runtime_error("Iterator out of range (too small)");
        
        while (_node->parent->left == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) throw std::runtime_error("Iterator out of range (too small)");
        _node = _node->parent;
    }
    else {
        _node = _node->left;
        while (_node->right)
        {
            _node = _node->right;
        }
        
    }
    return Iterator(oldnode, _root);
}


bool BinarySearchTree::Iterator::operator==(const BinarySearchTree::Iterator &other) const {
    return _node==other._node;
}


bool BinarySearchTree::Iterator::operator!=(const BinarySearchTree::Iterator &other) const {
    return _node!=other._node;
}


///------------------------------------


BinarySearchTree::ConstIterator::ConstIterator(const BinarySearchTree::Node* node,
const BinarySearchTree::Node* root) {
    _node = node;
    _root = root;
}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (!_node) throw std::runtime_error("End iterator is nullptr");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    if (!_node) throw std::runtime_error("End iterator is nullptr");
    return &(_node->keyValuePair);
}


BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node == nullptr) throw std::runtime_error("ConstIterator out of range (too big)");

    if (!_node->right) {
        if (!_node->parent) {_node = nullptr; return *this;}
        
        while (_node->parent->right == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) {_node = nullptr; return *this;}
        _node = _node->parent;
    }
    else {
        _node = _node->right;
        while (_node->left)
        {
            _node = _node->left;
        }
        
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    if (_node == nullptr) throw std::runtime_error("ConstIterator out of range (too big)");

    const Node* oldnode = _node;
    if (!_node->right) {
        if (!_node->parent) {_node = nullptr; return ConstIterator(oldnode, _root);;}
        
        while (_node->parent->right == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) {_node = nullptr; return ConstIterator(oldnode, _root);;}
        _node = _node->parent;
    }
    else {
        _node = _node->right;
        while (_node->left)
        {
            _node = _node->left;
        }
        
    }
    return ConstIterator(oldnode, _root);
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node == nullptr) {
        _node = _root;
        while (_node->right) {
            _node = _node->right;
        }
        
    }
    else if (!_node->left) {
        if (!_node->parent) throw std::runtime_error("ConstIterator out of range (too small)");
        
        while (_node->parent->left == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) throw std::runtime_error("ConstIterator out of range (too small)");
        _node = _node->parent;
    }
    else {
        _node = _node->left;
        while (_node->right)
        {
            _node = _node->right;
        }
        
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    const Node* oldnode = _node;
    if (_node == nullptr) {
        _node = _root;
        while (_node->right) {
            _node = _node->right;
        }
        
    }
    else if (!_node->left) {
        if (!_node->parent) throw std::runtime_error("ConstIterator out of range (too small)");
        
        while (_node->parent->left == _node)
        {
            _node = _node->parent;
            if (!_node->parent) break;
        }
        if (!_node->parent) throw std::runtime_error("ConstIterator out of range (too small)");
        _node = _node->parent;
    }
    else {
        _node = _node->left;
        while (_node->right)
        {
            _node = _node->right;
        }
        
    }
    return ConstIterator(oldnode, _root);
}


bool BinarySearchTree::ConstIterator::operator==(const BinarySearchTree::ConstIterator &other) const {
    return _node==other._node;
}


bool BinarySearchTree::ConstIterator::operator!=(const BinarySearchTree::ConstIterator &other) const {
    return _node!=other._node;
}




/// --------------------------------------------------


BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node* ptr = _root;
    while (key != ptr->keyValuePair.first) {
        if (key < ptr->keyValuePair.first) {
            if (!ptr->left) std::runtime_error("Key not in tree");
            ptr = ptr->left;
        }
        else {
            if (!ptr->right) std::runtime_error("Key not in tree");
            ptr = ptr->right;
        }
    }
    return ConstIterator(ptr, _root);
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node* ptr = _root;
    while (key != ptr->keyValuePair.first) {
        if (key < ptr->keyValuePair.first) {
            if (!ptr->left) std::runtime_error("Key not in tree");
            ptr = ptr->left;
        }
        else {
            if (!ptr->right) std::runtime_error("Key not in tree");
            ptr = ptr->right;
        }
    }
    return Iterator(ptr, _root);
}


std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
    if (!_root) throw std::runtime_error("Range in empty tree");
    Node* ptr = _root;
    std::cout << ptr << " key " << ptr->keyValuePair.first << " L: " << ptr->left << " R: " << ptr->right << std::endl;
    while (key != ptr->keyValuePair.first) {
        if (key < ptr->keyValuePair.first) {
            if (!ptr->left) std::runtime_error("Key not in tree");
            ptr = ptr->left;
        }
        else {
            if (!ptr->right) std::runtime_error("Key not in tree");
            ptr = ptr->right;
        }
    }
    Iterator first(ptr, _root);
    Iterator second(ptr, _root);
    Iterator fin = end();
    while (second != fin) {
        if (second->first != key) break;
        second++;
    }
    return std::pair<Iterator, Iterator>(first, second);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const {
    if (!_root) throw std::runtime_error("Range in empty tree");
    Node* ptr = _root;
    while (key != ptr->keyValuePair.first) {
        if (key < ptr->keyValuePair.first) {
            if (!ptr->left) std::runtime_error("Key not in tree");
            ptr = ptr->left;
        }
        else {
            if (!ptr->right) std::runtime_error("Key not in tree");
            ptr = ptr->right;
        }
    }
    ConstIterator first(ptr, _root);
    ConstIterator second(ptr, _root);
    ConstIterator fin = cend();
    while (second != fin) {
        if (second->first != key) break;
        second++;
    }
    return std::pair<ConstIterator, ConstIterator>(first, second);
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    Node* ptr = _root;
    while (ptr->left) {
        ptr = ptr->left;
    }
    return ConstIterator(ptr, _root);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    Node* ptr = _root;
    while (ptr->right) {
        ptr = ptr->right;
    }
    return ConstIterator(ptr, _root);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    std::pair<ConstIterator, ConstIterator> range = equalRange(key);
    ConstIterator minInt = range.first;
    for (auto i = range.first; i != range.second; i++) {
        if (i->second < minInt->second) minInt = i;
    }
    return minInt;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    std::pair<ConstIterator, ConstIterator> range = equalRange(key);
    ConstIterator maxInt = range.first;
    for (auto i = range.first; i != range.second; i++) {
        if (i->second > maxInt->second) maxInt = i;
    }
    return maxInt;
}


BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node* ptr = _root;
    while (ptr->left) {
        ptr = ptr->left;
    }
    return Iterator(ptr, _root);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    return Iterator(nullptr, _root);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node* ptr = _root;
    while (ptr->left) {
        ptr = ptr->left;
    }
    return ConstIterator(ptr, _root);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    return ConstIterator(nullptr, _root);
}


size_t BinarySearchTree::size() const {
    if (!_root) return 0;
    return _root->size();
}