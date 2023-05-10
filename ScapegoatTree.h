#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"
using namespace std;

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;
    

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    int getSize_helper(Node<T> *node) const
    {
        if (node == NULL)
        return 0;
        return 1 + getSize_helper(node->left) + getSize_helper(node->right);
    }
    int getHeight_helper(Node<T> *node) const
    {
        if(!node)
        return -1;
        else
        {
            int left_height = getHeight_helper(node->left);
            int right_height = getHeight_helper(node->right);
            if(left_height >= right_height)
               return (left_height + 1);
            else
                return (right_height + 1);
        }
    }
    void removeAll_helper(Node<T> *&node) const
    {
        if(node==NULL) return; 
        removeAll_helper(node->left);
        removeAll_helper(node->right);
        if(node->left==NULL && node->right==NULL)
        {
            delete node;
            node=NULL;
        }
    }
    const T &get_helper(Node <T> *node,T element) const
    {
        if(node==NULL)
            throw NoSuchItemException();
        else
        {
            if(node->element==element)
                return node->element;
            else if(node->element < element)
                return get_helper(node->right,element);
            else if(node->element > element)
                return get_helper(node->left,element);
        }
        throw NoSuchItemException();
    } 
    bool contains(Node <T> *node,const T &element) const
    {
        if(node==NULL)
        {
            return false;
        }
        else
        {
            if(node->element==element)
            {
                return true;
            }
            else
            return (contains(node->right,element) || contains(node->left,element));
        }
    } 
    Node<T> *insert_helper(Node <T> * &node,const T &element) 
    {
        if(node==NULL)
        {
            node = new Node<T>(element,NULL,NULL);
            return node;
        }
        else
        {
            if(element < node->element)
            {
                return insert_helper(node->left,element);
            }
            else if(element > node->element)
            {
                return insert_helper(node->right,element);
            }
           
        }
    }
    int log_helper(int upperbound) const
    {
        return (float)log(upperbound)/(float)log(1.5);
        
    }

    void balancehelper(Node<T> *node)
    {
        int s=getSize_helper(node);
        T *arr=new T[s];
        int i=0;
        inorderarray(arr,node,i);
        Node <T> *child = construct_tree(arr,0,s-1);
        Node <T> *parent = getparent(node,root);
        if(parent==NULL) 
        {
            delete[] arr;
            removeAll_helper(node);
            root=child;
            return;
        }
        if(child->element < parent->element)
        {
            parent->left = child;
        }  
        else parent->right =child;
        delete[] arr;
        removeAll_helper(node);
    }
    void inorderarray(T array[],Node <T> *node,int &i) const
    {
        if(node==NULL) return;
        inorderarray(array,node->left,i);
        array[i]=node->element;
        i++;
        inorderarray(array,node->right,i);
    }
    Node<T> *construct_tree(T array[],int start,int end) const
    {
        if(start > end)
        return NULL;
        int mid = (start+end)/2;
        Node <T> *newnode =new Node<T>;
        newnode->element=array[mid];

        newnode->left=construct_tree(array,start,mid-1);

        newnode->right=construct_tree(array,mid+1,end);
        
        return newnode;
    }
    Node<T> *getparent(Node <T> *node,Node <T> *r) const
    {
        if(node==NULL) return NULL;
        if(node==r) return NULL;
        if(r->right == node || r->left == node)
            return r;
        if(r->element > node->element)
            return getparent(node,r->left);
        if(r->element < node->element)
            return getparent(node,r->right);
    }
    Node<T> *findscapegoat(Node<T> *node)
    {
        int s1=getSize_helper(node);
        Node <T> *parent= getparent(node,root);
        int s2=getSize_helper(parent);
        if((float)s1/(float)s2 > 0.667)
        {
            return parent;
        }
        else findscapegoat(parent);
    }
    Node<T> *copytree(Node <T> * &r,Node<T> *node)
    {
        if(node==NULL)
        {
            r=NULL;
            return NULL;
        }
        Node<T> *newnode = new Node<T>(node->element,NULL,NULL);
        
        
        copytree(newnode->left,node->left);
        copytree(newnode->right,node->right);

        r=newnode;
        return r;
    }
    T &getminhelper(Node<T> *r) const
    {
        if(r==NULL)
        {
            throw NoSuchItemException();
        }
        while(r->left!=NULL)
        {
            r=r->left;
        }
        return r->element;
    }
    T &getmaxhelper(Node<T> *r) const
    {
        if(r==NULL)
        {
            throw NoSuchItemException();
        }
        while(r->right!=NULL)
        {
            r=r->right;
        }
        return r->element;
    }
    Node<T> *ceilinghelper(T element,Node <T> *r) const
    {
        if(r==NULL) return NULL;
        if(r->element==element) return r;
        
        if(r->element < element)
        {
            return ceilinghelper(element,r->right);
        }
        if(r->element > element)
        {
            Node<T> *p =ceilinghelper(element,r->left);
            if(p==NULL) return r;
            return p;
        }
    }
    Node<T> *floorhelper(T element,Node <T> *r) const
    {
        if(r==NULL) return NULL;
        if(r->element==element) return r;
        if(r->element > element)
        {
            return floorhelper(element,r->left);
        }
        if(r->element < element)
        {
            Node<T> *p =floorhelper(element,r->right);
            if(p==NULL) return r;
            return p;
        }
        
    }
    Node<T> *getnexthelper(T element,Node <T> *r) const
    {
        if(r==NULL) return NULL;
        
        if(r->element <= element)
        {
            Node<T> *p = getnexthelper(element,r->right);
            return p;
        }
        if(r->element > element)
        {
            Node<T> *p =getnexthelper(element,r->left);
            if(p==NULL) return r;
            return p;
        }
    }
    void removehelper(Node<T> *& r, T value) {
          if (r == NULL)
             return;
          else if (r->element > value)
             removehelper(r->left, value);
          else if (r->element < value)
             removehelper(r->right , value);
          else    // r->data == value; remove this node
             if (r->left !=NULL && r->right != NULL) {
               // both children; replace w/ min from R
                r->element = getmaxhelper(r->left);
                removehelper(r->left, r->element);
               }
             else {
                Node<T> * oldNode =r;  // only  R or L child
                r = (r->left != NULL)? r->left : r->right;
                delete oldNode;
             }
        }
    
    
private: // DO NOT CHANGE THIS PART.
    Node<T> *root;
    int upperBound;
};

#endif //TREE_H

template<class T>
ScapegoatTree<T>::ScapegoatTree() {
    root=NULL;
    upperBound=0;
    /* TODO */
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj) {
    root=NULL;
    *this=obj;
    /* TODO */
}

template<class T>
ScapegoatTree<T>::~ScapegoatTree() {
    removeAllNodes();
    upperBound=0;
    /* TODO */
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const {
    return root==NULL;
    /* TODO */
}

template<class T>
int ScapegoatTree<T>::getHeight() const {
    return getHeight_helper(root);
}

template<class T>
int ScapegoatTree<T>::getSize() const {
    return getSize_helper(root);
    /* TODO */
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element) {
    if(contains(root,element))
    {
        return false;
    }
    Node<T> *p =insert_helper(root,element);
    upperBound++;
    int hei=getHeight_helper(root);
    if(hei > log_helper(upperBound))
    {
        Node <T> *scapegoat =findscapegoat(p);
        balancehelper(scapegoat);
        return true;
    }
    else return true;
    /* TODO */
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element) {
    if(root==NULL) return false;
    if(!contains(root,element))
    {
        return false;
    }
    removehelper(root,element);
    int si =getSize();
    if((float)upperBound/(float)2 > si)
    {
        balance();
        int s=getSize();
        upperBound=s;
    }
    
    return true;
    
    /* TODO */
}

template<class T>
void ScapegoatTree<T>::removeAllNodes() {
    removeAll_helper(root);
    upperBound=0;
    /* TODO */
}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const {
    return get_helper(root,element);
    /* TODO */
}

template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const {

    if (tp == preorder) {
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
        /* TODO */
    } else if (tp == inorder) {
        // check if the tree is empty?
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder) {
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
        /* TODO */
        /* TODO */
    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const {

    if (tp == preorder) {
        if(node==NULL)
            return;
        std::cout << "\t" << node->element;
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->left, preorder);
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);
        /* TODO */
    } else if (tp == inorder) {
        // check if the node is NULL?
        if (node == NULL)
            return;

        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }

        // then, output the node.
        std::cout << "\t" << node->element;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } else if (tp == postorder) {
        if(node==NULL)
            return;
        print(node->left, postorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->right, postorder);
        if (node->right) {
            std::cout << "," << std::endl;
        }
        std::cout << "\t" << node->element;
        /* TODO */
    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const {

    // check if the tree is empty?
    if (isEmpty()) {
        // the tree is empty.
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }

    // the tree is not empty.

    // recursively output the tree.
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;

}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const {

    // check if the node is NULL?
    if (node == NULL)
        return;

    // output the indentation and the node.
    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    // first, output left subtree with one more indentation level.
    printPretty(node->left, indentLevel + 1, true);

    // then, output right subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false);
}

template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs) {
    if(this != &rhs)
    {
        if(rhs.root == NULL)
        {
            this->root = NULL;
        }
        else{
            this->removeAllNodes();
            copytree(root,rhs.root);
        }
        upperBound = rhs.upperBound;
    }
    return *this;
    /* TODO */
}

template<class T>
void ScapegoatTree<T>::balance() {
    balancehelper(root);
    /* TODO */
}

template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const {
    Node<T> *pp =ceilinghelper(element,root);
    if(pp==NULL)
    {
        throw NoSuchItemException();
    }
    return pp->element;
    
    /* TODO */
}

template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const {
    Node<T> *pp =floorhelper(element,root);
    if(pp==NULL)
    {
        throw NoSuchItemException();
    }
    return pp->element;
    
    /* TODO */
}

template<class T>
const T &ScapegoatTree<T>::getMin() const {
    getminhelper(root);
    /* TODO */
}

template<class T>
const T &ScapegoatTree<T>::getMax() const {
    getmaxhelper(root);
    /* TODO */
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const {
    Node<T> *pp =getnexthelper(element,root);
    if(pp==NULL)
    {
        throw NoSuchItemException();
    }
    return pp->element;
  
    /* TODO */
}


//HELPER FUNCTIONS


