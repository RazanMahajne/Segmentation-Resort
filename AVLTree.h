//
// Created by Yasmeena Khalaf on 07/06/2026.
//

#ifndef DS_WET1_SPRING_2026_AVLTREE_H
#define DS_WET1_SPRING_2026_AVLTREE_H

#include "TreeNode.h"

template<typename K, typename V>
class AVLTree {
private:
    TreeNode<K, V>* root;
    int nodesCount;
    //to get the height
    static int nodeH(TreeNode<K, V>* n) {
        return n ? n->getHeight() : 0;
    }
    //to calculate the BF-balance factor as learned
    static int bFactor(TreeNode<K, V>* n) {
        if (!n) {
            return 0;
        }
        return nodeH(n->getLeft()) - nodeH(n->getRight());
    }


    //updating the height
    static void updateH(TreeNode<K, V>* n) {
        if (!n) {
            return;
        }
        int leftchild = nodeH(n->getLeft());
        int rightChild = nodeH(n->getRight());
        n->setHeight(1 + (leftchild > rightChild ? leftchild : rightChild));
    }

    //get back to rotations implementation
    static TreeNode<K, V>* rotateRight(TreeNode<K, V>* y) {
        TreeNode<K, V>* x  = y->getLeft();
        TreeNode<K, V>* z  = x->getRight();
        x->setRight(y);
        y->setLeft(z);
        //x takes y place in the tree
        x->setParent(y->getParent());
        y->setParent(x);
        if (z) {
            z->setParent(y);
        }
        updateH(y); //y is now lower
        updateH(x); //also to update the height of x
        return x;
    }

    static TreeNode<K, V>* rotateLeft(TreeNode<K, V>* x) {
        TreeNode<K, V>* y  = x->getRight();
        TreeNode<K, V>* z  = y->getLeft();
        // y takes x place in the tree
        y->setLeft(x);
        x->setRight(z);
        y->setParent(x->getParent());
        x->setParent(y);
        if (z) {
            z->setParent(x);
        }
        updateH(x);
        updateH(y);
        return y;
    }

    //left left Case LL
    static TreeNode<K, V>* balanceLL(TreeNode<K, V>* node) {
        return rotateRight(node);
    }
    //left right case LR
    static TreeNode<K, V>* balanceLR(TreeNode<K, V>* node) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }


    //right right case RR
    static TreeNode<K, V>* balanceRR(TreeNode<K, V>* node) {
        return rotateLeft(node);
    }
    //right left case RL
    static TreeNode<K, V>* balanceRL(TreeNode<K, V>* node) {
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }

    // main rebalance function
    static TreeNode<K, V>* rebalance(TreeNode<K, V>* node) {
        updateH(node);
        int bf = bFactor(node);

        if (bf > 1) { // left heavy
            if (bFactor(node->getLeft()) < 0) {
                return balanceLR(node);
            }
            return balanceLL(node);
        }
        if (bf < -1) { // right heavy
            if (bFactor(node->getRight()) > 0) {
                return balanceRL(node);
            }
            return balanceRR(node);
        }
        return node;  //already balanced
    }

    static TreeNode<K, V>* insertAux(TreeNode<K, V>* node,
        TreeNode<K, V>* par, const K& key, const V& value,
        TreeNode<K, V>*& newNode) {

        if (!node) {
            node = new TreeNode<K, V>(key, value);
            node->setParent(par);
            newNode = node;
            return node;
        }
        // if its smaller insert left
        if (key < node->getKey()) {
            node->setLeft(insertAux(node->getLeft(), node, key, value, newNode));
        // if its bigger insert right recursivly
        } else if (key > node->getKey()) {
            node->setRight(insertAux(node->getRight(), node, key, value, newNode));
        } else {
            newNode = nullptr; // duplicate key we shouldn't insert
            return node;
        }

        return rebalance(node);//need to balance after
    }

    // maximum key node in the subtree rooted at node
    static TreeNode<K, V>* subtreeMax(TreeNode<K, V>* node) {
        if (!node) {
            return nullptr;
        }
        while (node->getRight()) {
            node = node->getRight();
        }
        return node;
    }

    // minimum key node in the subtree rooted at node
    static TreeNode<K, V>* subtreeMin(TreeNode<K, V>* node) {
        if (!node) {
            return nullptr;
        }
        while (node->getLeft()) {
            node = node->getLeft();
        }
        return node;
    }
    //I should check it again
    static TreeNode<K, V>* removeAux(TreeNode<K, V>* node, const K& key,
    bool& found) {
        if (!node) {
            found = false;
            return nullptr;
        }
        if (key < node->getKey()) {
            node->setLeft(removeAux(node->getLeft(), key, found));
        } else if (key > node->getKey()) {
            node->setRight(removeAux(node->getRight(), key, found));
        } else {
            // this is the node to delete
            found = true;
            if (!node->getLeft() || !node->getRight()) {
                // zero or one child
                TreeNode<K, V>* child = node->getLeft() ? node->getLeft() : node->getRight();
                if (child) {
                    child->setParent(node->getParent());
                    updateH(child); //ensure the shifted node recalculates its relative depth
                }
                delete node;
                return child;
            }
            //if with 2 children we replace content with inorder successor then
            //delete the successor from the right subtree
            TreeNode<K, V>* succ = subtreeMin(node->getRight());
            node->setKey(succ->getKey());
            node->setValue(succ->getValue());
            node->setRight(removeAux(node->getRight(),succ->getKey(),
            found));
        }
        return rebalance(node); // and of course to balance
    }
    //to fill the array inorder
    static void inOrderAux(TreeNode<K, V>* node, TreeNode<K, V>** arr,
    int& indx) {
        if (!node) {
            return;
        }
        inOrderAux(node->getLeft(),  arr, indx);
        arr[indx++] = node;
        inOrderAux(node->getRight(), arr, indx);
    }

    //to delete a subtree from memory without causing memory leak
    //should use it in ~Segmentation
    static void postFreeHelper(TreeNode<K, V>* node) {
        if (!node) {
            return;
        }
        postFreeHelper(node->getLeft());
        postFreeHelper(node->getRight());
        delete node;
    }
    static TreeNode<K, V>* buildHelper(TreeNode<K, V>** arr, int low, int high,
    TreeNode<K, V>* p) {
        if (low > high) {
            return nullptr;
        }
        int mid = low + (high - low) / 2;
        TreeNode<K, V>* node = arr[mid];
        node->setParent(p);
        node->setLeft(buildHelper(arr, low, mid - 1, node));
        node->setRight(buildHelper(arr, mid + 1, high, node));
        updateH(node);
        return node;
    }

public:
    AVLTree() : root(nullptr), nodesCount(0) {}
    //does not free nodes automatically we must call postOrderFree()
    // if we want memory cleanup this is intentional for cases where
    // nodes are shared like in joinTables
    ~AVLTree() {}
    TreeNode<K, V>* insert(const K& key, const V& value) {
        TreeNode<K, V>* newNode = nullptr;
        root = insertAux(root, nullptr, key, value, newNode);
        if (root) {
            root->setParent(nullptr);// root has no parent make sure of it
        } // root never has a parent
        if (newNode) {
            nodesCount++;
        }
        return newNode;
    }
    bool remove(const K& key) {
        bool found = false;
        root = removeAux(root, key, found);
        if (root) {
            root->setParent(nullptr);
        }
        if (found) {
            nodesCount--;
        }
        return found;
    }

    TreeNode<K, V>* find(const K& key) const {
        TreeNode<K, V>* cur = root;
        while (cur) {
            if (key < cur->getKey()) {
                cur = cur->getLeft();
            }
            else if (key > cur->getKey()) {
                cur = cur->getRight();
            }
            else {
                return cur;
            }
        }
        return nullptr;
    }

    TreeNode<K, V>* findMin() const {
        return subtreeMin(root);
    }

    TreeNode<K, V>* findMax() const {
        return subtreeMax(root);
    }

    TreeNode<K, V>* successor(TreeNode<K, V>* node) const {
        if (!node) {
            return nullptr;
        }
        if (node->getRight()) {
            return subtreeMin(node->getRight());
        }
        TreeNode<K, V>* curr = node;
        TreeNode<K, V>* p = node->getParent();
        while (p && curr == p->getRight()) {
            curr = p;
            p = p->getParent();
        }
        return p;
    }

    TreeNode<K, V>* predecessor(TreeNode<K, V>* node) const {
        if (!node) {
            return nullptr;
        }
        if (node->getLeft()) {
            return subtreeMax(node->getLeft());
        }
        TreeNode<K, V>* curr = node;
        TreeNode<K, V>* p = node->getParent();
        while (p && curr == p->getLeft()) {
            curr = p;
            p = p->getParent();
        }
        return p;
    }
    void buildFromSortedArray(TreeNode<K, V>** arr, int n) {
        root = buildHelper(arr, 0, n - 1, nullptr);
        nodesCount = n;
    }

    void inOrderToArray(TreeNode<K, V>** arr) const {
        int indx = 0;
        inOrderAux(root, arr, indx);
    }
    //to free the AVL in inorder we will use it in ~segmentation
    void postOrderFree() {
        postFreeHelper(root);
        root = nullptr;
        nodesCount = 0;
    }

    bool isEmpty() const {
        return root == nullptr;
    }
    // getters
    TreeNode<K, V>* getRoot() const {
        return root;
    }
    int getSize()  const {
        return nodesCount;
    }
};

/* for debug
template<typename K, typename V>
static void printInOrder(TreeNode<K,V>* node)
{
    if (!node) {
        return;
    }
    printInOrder(node->getLeft());
    std::cout << node->getKey() << " ";
    printInOrder(node->getRight());
}
void printTree() const
{
    printInOrder(root);
    std::cout << std::endl;
}
*/


#endif //DS_WET1_SPRING_2026_AVLTREE_H
