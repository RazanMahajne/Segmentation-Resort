//
// Created by Yasmeena Khalaf on 07/06/2026.
//

#ifndef DS_WET1_SPRING_2026_TREENODE_H
#define DS_WET1_SPRING_2026_TREENODE_H

template<typename K, typename V>
class TreeNode {
private:
    K key;
    V value;
    TreeNode<K, V>* left;
    TreeNode<K, V>* right;
    TreeNode<K, V>* parent;
    int height;

public:
    TreeNode(const K& k, const V& v) : key(k), value(v), left(nullptr),
    right(nullptr), parent(nullptr), height(1) {}
    // getters
    const K& getKey() const {
        return key;
    }
    const V& getValue() const {
        return value;
    }
    V& getValue() {
        return value;
    }
    TreeNode<K, V>* getLeft() const {
        return left;
    }
    TreeNode<K, V>* getRight() const {
        return right;
    }
    TreeNode<K, V>* getParent() const {
        return parent;
    }
    int getHeight() const {
        return height;
    }

    //setters
    void setKey(const K& k) {
        key = k;
    }
    void setValue(const V& v) {
        value = v;
    }
    void setLeft(TreeNode<K, V>* l) {
        left = l;
    }
    void setRight(TreeNode<K, V>* r) {
        right = r;
    }
    void setParent(TreeNode<K, V>* p) {
        parent = p;
    }
    void setHeight(int h) {
        height = h;
    }
};

#endif //DS_WET1_SPRING_2026_TREENODE_H
