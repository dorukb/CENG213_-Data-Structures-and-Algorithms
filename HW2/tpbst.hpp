#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);

    T* findHelper(PrimaryNode* &root,const std::string &primaryKey, const std::string &secondaryKey);
    T* findHelper_2(SecondaryNode* &node,const std::string & secondaryKey);

    void insertHelper(PrimaryNode* &node,const std::string & primaryKey, const std::string & secondaryKey, const T & data);
    void insertHelper_2(SecondaryNode* &node, const std::string &secondaryKey, const T& data);

    void printAll(PrimaryNode* root);
    void printAllHelper_1(PrimaryNode* node, const std::string &minKey);
    void printAllHelper_2(SecondaryNode* node,const std::string & minKey);
    void printCase2Helper(PrimaryNode* node,const std::string& primaryKey);
    void printCase2Helper2(SecondaryNode* node);

    void removeHelper(PrimaryNode* &node,const std::string &primaryKey, const std::string &secondaryKey);
    void removeHelper_2(SecondaryNode* &node, const std::string & secondaryKey,const std::string & primaryKey);
    TwoPhaseBST<T>::SecondaryNode* findMin(SecondaryNode *node) const;
    TwoPhaseBST<T>::PrimaryNode*  findMin2(PrimaryNode *node) const;




private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {
    /* IMPLEMENT THIS */
    if(root == NULL){ //empty tree.
        root = new PrimaryNode(primaryKey,NULL,NULL, NULL);
        root->rootSecondaryNode = new SecondaryNode(secondaryKey,data,NULL,NULL);
    }
    else{
        insertHelper(root,primaryKey, secondaryKey, data);
    }
    return *this;
}

template <class T>
void TwoPhaseBST<T>::insertHelper(PrimaryNode* &node,const std::string & primaryKey, const std::string & secondaryKey, const T & data){
    if(node == NULL){
        node = new PrimaryNode(primaryKey,NULL,NULL, NULL);
        node->rootSecondaryNode = new SecondaryNode(secondaryKey,data,NULL,NULL);
    }
    else if( primaryKey > node->key){
        insertHelper(node->right, primaryKey, secondaryKey, data);
    }
    else if( primaryKey < node->key){
        insertHelper(node->left, primaryKey, secondaryKey, data);
    }
    else{ // matches with curr`s key. So insert it to this primary node`s secondary tree.
        insertHelper_2(node->rootSecondaryNode, secondaryKey, data);
    }
}

template <class T>
void TwoPhaseBST<T>::insertHelper_2(SecondaryNode* &node, const std::string &secondaryKey, const T& data){
    if(node == NULL){
        node = new SecondaryNode(secondaryKey, data, NULL, NULL);
    }
    if( secondaryKey > node->key){
        insertHelper_2(node->right,secondaryKey, data);
    }
    else if( secondaryKey < node->key){
        insertHelper_2(node->left,secondaryKey, data);
    }
    else{ // matches with this one?
        // duplicate.. dont do anything.
    }
}


template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    removeHelper(root,primaryKey,secondaryKey);
    return *this;
}

template <class T> // FIND the PRIMARY_NODE that has the secondarytree to be removed.
    void TwoPhaseBST<T>::removeHelper(PrimaryNode* &node,const std::string &primaryKey, const std::string &secondaryKey){
    if(node == NULL){return;}
    if(primaryKey > node->key){ //search Right subtree;
        return removeHelper(node->right,primaryKey,secondaryKey);
    }
    else if(primaryKey < node->key){
        return removeHelper(node->left,primaryKey,secondaryKey);
    }
    else{// we found the primaryNode. now search for the node?
        return removeHelper_2(node->rootSecondaryNode, secondaryKey, primaryKey);
    }
}
template <class T>
void TwoPhaseBST<T>::removeHelper_2(SecondaryNode* &node, const std::string & secondaryKey, const std::string & primaryKey){
    if(node == NULL) {return;}
    if(secondaryKey > node->key ){
        removeHelper_2(node->right, secondaryKey, primaryKey);
    }
    else if(secondaryKey < node->key){
        removeHelper_2(node->left, secondaryKey , primaryKey);
    }
    else if(node->left && node->right){ // both child exist.
         // we will re-create the deleted node. we cant swap data fields cuz of "const" variables.
        const std::string secKey = findMin( node->right ) -> key;
        T its_data               = findMin( node->right ) -> data;
        remove(primaryKey, secKey);
        SecondaryNode* tmp2 = node->right;
        SecondaryNode* tmp1 = node->left;
        delete node;
        node = new SecondaryNode(secKey, its_data, tmp1, tmp2);
         // delete the one that will replace the current
    }
    else{ // 1 child case
        SecondaryNode* tmp = node;
        node  = ( node->left != NULL ) ? node->left : node->right;
        delete tmp;
        }
    }


template<class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findMin(SecondaryNode *node) const{
    if(node != NULL){
        while(node->left){
            node = node->left;
        }
    }
    return node;
}

template<class T>
typename TwoPhaseBST<T>::PrimaryNode* TwoPhaseBST<T>::findMin2(PrimaryNode *node) const{
    if(node != NULL){
        while(node->left){
            node = node->left;
        }
    }
    return node;
}


template <class T>
void TwoPhaseBST<T>::printAll(PrimaryNode* root){
    if(root == NULL){ std::cout << "{}" << std::endl;} // empty tree.
    else{
        std::cout <<'{';
        PrimaryNode* minNode = findMin2(root);
        if(minNode != NULL){
            const std::string minKey = minNode-> key;
            printAllHelper_1(root,minKey);
        }
        else{
            printAllHelper_1(root,"emtpty");
        }

        std::cout << '}';
        std::cout << std::endl;
    }
}
template <class T>
void TwoPhaseBST<T>::printAllHelper_2(SecondaryNode* node, const std::string & key){
    if(node == NULL){return;}

    printAllHelper_2(node->left,key);
    if(node->key == key)
    {
      std::cout<<'"' << node->key <<'"' << " : " << '"' << node->data << '"';
    }
    else{
      std::cout<< ", " <<'"' << node->key <<'"' << " : " << '"' << node->data << '"';
    }

    printAllHelper_2(node->right,key);
}
template <class T>
void TwoPhaseBST<T>::printAllHelper_1(PrimaryNode* node,const std::string & minKey){
    if(node == NULL){return;}

    printAllHelper_1(node->left,minKey);

    if(node->key != minKey){
      std::cout<<", ";
    }

    std::cout << '"'<< node->key << '"' << " : " << '{';

    SecondaryNode* minNode = findMin(node->rootSecondaryNode);
    if(minNode != NULL){
      const std::string secKey = minNode -> key;
      printAllHelper_2(node->rootSecondaryNode,secKey);
      std::cout << '}';
    }
    else{
      printAllHelper_2(node->rootSecondaryNode,"empty_case");
      std::cout << '}';
    }

    printAllHelper_1(node->right,minKey);
}

template <class T>
void TwoPhaseBST<T>::printCase2Helper(PrimaryNode* node,const std::string& primaryKey){
    if(node == NULL){return;}
    if(primaryKey > node->key){
        printCase2Helper(node->right, primaryKey);
    }
    else if(primaryKey < node->key){
        printCase2Helper(node->left, primaryKey);
    }
    else{ // found it.
        std::cout << '"'<< node->key << '"' << " : " << '{';
        SecondaryNode* minNode = findMin(node->rootSecondaryNode);
        if(minNode != NULL){
          const std::string secKey = findMin(node->rootSecondaryNode) -> key;
          printAllHelper_2(node->rootSecondaryNode, secKey);
          std::cout << '}';
        }
        else
        {
          printAllHelper_2(node->rootSecondaryNode, "empty_case");
          std::cout << '}';
        }

    }
}

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    if(primaryKey == "" && secondaryKey == ""){ // print the whole  tpbst.
        printAll(root);
    }
    else if(secondaryKey == ""){ //print all that is in the PrimaryNode with given primaryKey.
        std::cout <<'{';
        if(root != NULL){
            printCase2Helper(root,primaryKey);
        }
        std::cout <<'}';
        std::cout << std::endl;
    }
    else if(primaryKey == ""){;} // dont do anything.
    else{ // both keys are given. print a specific node.
        T* data = find(primaryKey, secondaryKey);
        if(data == NULL){std::cout << "{}" << std::endl;} // not found.
        else{
            std::cout << "{" << '"'<< primaryKey << '"'<< " : " << "{" << '"' <<secondaryKey << '"' <<" : "<<'"'<< (*data) << '"' <<"}}" << std::endl;
        }
    }
    return *this;
}

template <class T>
    T* TwoPhaseBST<T>::findHelper(PrimaryNode* &node,const std::string &primaryKey, const std::string &secondaryKey){
    if(node == NULL){return NULL;}
    if(primaryKey > node->key){ //search Right subtree;
        return findHelper(node->right,primaryKey,secondaryKey);
    }
    else if(primaryKey < node->key){
        return findHelper(node->left,primaryKey,secondaryKey);
    }
    else{//then it is equal to node key.
        return findHelper_2(node->rootSecondaryNode, secondaryKey);
    }
}
template <class T>
T* TwoPhaseBST<T>::findHelper_2(SecondaryNode* &node, const std::string &secondaryKey){
    if(node == NULL){return NULL;}
    if(secondaryKey > node->key){
        return findHelper_2(node->right, secondaryKey);
    }
    else if(secondaryKey < node->key){
        return findHelper_2(node->left, secondaryKey);
    }
    else{ // then equals to this guys` data.
        return &(node->data);
    }
}
template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    return findHelper(root,primaryKey,secondaryKey);
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

#endif //TWOPHASEBST_H

