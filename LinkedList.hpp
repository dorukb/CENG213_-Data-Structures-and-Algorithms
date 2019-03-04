#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head;
    int length;
public:
    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data);
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/
/* TO-DO: method implementations below */
template <typename T>
Node<T>* LinkedList<T>::getHead() const{return head;}
template <typename T>
Node<T>* LinkedList<T>::first() const{return head->getNext();}//since head is of type Node! and next is private in Node class. must use Getter func!
template <typename T>
Node<T>* LinkedList<T>::findPrev(const T& data) const
{
  Node<T>* ptr = getHead();
  while(ptr->getNext())
  {
    if(ptr->getNext()->getData() == data){return ptr;}
    ptr = ptr->getNext();
  }
  return NULL;
}
template <typename T>
Node<T>* LinkedList<T>::findNode(const T& data) const
{
  Node<T>* ptr = first();
  while(ptr)
  {
    if(ptr->getData() == data){return ptr;}
    ptr = ptr->getNext();
  }
  return NULL;
}
template <typename T> // INSERTION TO THE BEGINNING !!
void LinkedList<T>::insertNode(Node<T>* prev, const T& data)
{
  if(prev)
  {
    Node<T> *newNode = new Node<T>(data);
    newNode->setNext(prev->getNext());
    prev->setNext(newNode);
    length++;
  }
} // INSERTION TO THE BEGINNING !
template <typename T>
void LinkedList<T>::deleteNode(Node<T>* prevNode)
{
  if(prevNode) // prevNode is not NULL.
  {
    if(prevNode -> getNext()) // if there is sth to delete actually.
    {
      Node<T> *tmp = prevNode->getNext();
      prevNode->setNext(tmp->getNext());
      length--;
      delete tmp;
    }
  }
}
template <typename T>
void LinkedList<T>::clear()
{
  Node<T>* ptr = getHead();
  if(ptr)
  {
    while(ptr->getNext())//there is an element to delete.
    {
      deleteNode(ptr);
    }
  }
}
template <typename T>
size_t LinkedList<T>::getLength() const{return length;}

template <typename T>
LinkedList<T>::LinkedList()
{
  head = new Node<T>(T());
  length = 0;
}
template <typename T> // copy constructor !!
// nothing to clean! this is the initialization of our list.
LinkedList<T>::LinkedList(const LinkedList<T>& ll)
{
  Node<T> *fromptr = ll.first();
  head = new Node<T> (T());
  length = ll.getLength();
  Node<T> *toptr = getHead();
  while(fromptr)
  {
    insertNode(toptr, fromptr->getData());
    toptr = toptr -> getNext();
    fromptr = fromptr ->getNext();
  }
}

template <typename T>
void LinkedList<T>::swap(int index1, int index2)
{
    if(index2 != index1 && (index1 < length && index2 < length))
    {
        Node<T> *ptr1 = getHead();
        Node<T> *ptr2 = getHead();
        for(int i=0; i <index1; i++)
        {
            ptr1 = ptr1->getNext();
        }
        for(int i=0; i<index2; ++i)
        {
            ptr2 = ptr2->getNext();
        }
        if(index1 > index2) // ensure that PTR2 ALWAYS POINTS to the'FAR' node
        {                   // so that our 'neighbor swap' works.
            Node<T> *tp = ptr1;
            ptr1 = ptr2;
            ptr2 = tp;
        }
        Node<T> *tmp1,*tmp2;
        if((index2 - index1 > 1) || (index1 - index2 > 1))//NOT NEIGHBORS!
        {
            tmp2 = ptr2->getNext()->getNext();
            ptr2->getNext()->setNext(ptr1->getNext()->getNext());
            ptr1->getNext()->setNext(tmp2);
            tmp1 = ptr1->getNext();
            ptr1->setNext(ptr2->getNext());
            ptr2->setNext(tmp1);
        }
        else//NODES ARE ADJACENT!!
        {
            tmp2 = ptr2->getNext()->getNext();
            tmp1 = ptr2->getNext();
            ptr2 -> setNext(tmp2);
            ptr1 -> setNext(tmp1);
            tmp1->setNext(ptr2);
        }
    }
}

// Assignment operator ! need to clean the list first !
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll)
{
    if( this != &ll)
    {
        clear();
        Node<T> *fromptr = ll.first();
        length = ll.getLength();
        Node<T> *toptr = getHead();
        while(fromptr)
        {
          insertNode(toptr, fromptr->getData());
          toptr = toptr -> getNext();
          fromptr = fromptr ->getNext();
        }
    
    }
    return *this;
}
template <typename T>
LinkedList<T>::~LinkedList()
{
  clear();
  delete getHead();
}


/* end of your implementations*/
#endif
