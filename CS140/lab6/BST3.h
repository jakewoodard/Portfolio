#ifndef BST_H
#define BST_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
 
template <class TKey>
class bst {
  struct node {
    node(int ID = 0);

    void print();

    TKey key;
    int ID; // unique ID for each node
    
    // parent and child nodes
    node *parent;
    node *left;
    node *right;
  };
  
  public:
    class iterator {
      public:
        // constructor (public)
        iterator();

        // operator overloads
        iterator & operator++();
        TKey & operator*();
        bool operator==(const iterator &) const;
        bool operator!=(const iterator &) const;
      private:
        friend class bst<TKey>;

        // constructor (private)
        iterator(node *);

        // iterator pointer
        node *p;
    };

  public:
    bst() { I = 0; Troot=NULL; }
    ~bst() { clear(Troot); }

    bool empty() { return Troot==NULL; }

    void insert(TKey &);

    void print_bylevel();

    iterator begin();
    iterator end();
    
    iterator lower_bound(const TKey &);
    iterator upper_bound(const TKey &);
  private:
    void clear(node *);
    node *insert(node *, TKey &);

    int I = 0;
    node *Troot;
};

// node constructor
template <class TKey>
bst<TKey>::node::node(int x)
{
  ID = x;
  parent = NULL;
  left = NULL;
  right = NULL;
}

template <class TKey>
void bst<TKey>::node::print()
{
  cout << setw(3) << ID;
  cout << setw(3) << key << " : ";
  if (parent == NULL) cout << "ROOT ";
  else               cout << "P=  " << parent->ID;

  // output node and parent ID information
  // change below to output subtree ID information

  if (left)  cout << " L=" << setw(3) << left->ID;
  else       cout << "      ";
  if (right) cout << " R=" << setw(3) << right->ID;
  else       cout << "      ";

  cout << "\n";
}

// specialized string version of the above goes here
template <>
void bst<string>::node::print()
{
  cout << setw(3) << ID;
  cout << setw(20) << key << " : ";
  if (parent == NULL) cout << "ROOT ";
  else               cout << "P=  " << parent->ID;

  // output node and parent ID information
  // change below to output subtree ID information

  if (left)  cout << " L=" << setw(3) << left->ID;
  else       cout << "      ";
  if (right) cout << " R=" << setw(3) << right->ID;
  else       cout << "      ";

  cout << "\n";
}

template <class TKey>
bst<TKey>::iterator::iterator() {
  p = NULL;
}

template <class TKey>
bst<TKey>::iterator::iterator(bst<TKey>::node *ptr) {
  p = ptr;
}

template <class TKey>
typename bst<TKey>::iterator & bst<TKey>::iterator::operator++() {
  
  // if (p != NULL) cout << setw(3) << p->key; 
  
  // if at root and right is null
  if ((p->right == NULL) && (p->parent == NULL)) {
    p = NULL;
    return *this;
  }
  
  // check if we can go right
  // if so go right once
  if (p->right != NULL) { 
    p = p->right;
    // go left as much as possible
    while (p->left != NULL) {
      p = p->left;
    }
    return *this;
  }

  // otherwise, go up to parent, if the node we
  // were just at is the right child of the node
  // we are now at, go up again
  node *temp;
  do {
    temp = p;
    p = p->parent;  
  } while ((temp == p->right) && (p->parent != NULL));
  
  if (temp == p->left) 
    { return *this; }
  if (p->parent == NULL)
    p = NULL;

  return *this;
}

template <class TKey>
TKey & bst<TKey>::iterator::operator*() {
  return p->key;
}

template <class TKey>
bool bst<TKey>::iterator::operator==(const bst<TKey>::iterator &rhs) const {
  if (p == rhs.p) return true;  return false;
}

template <class TKey>
bool bst<TKey>::iterator::operator!=(const bst<TKey>::iterator &rhs) const {
  if (p != rhs.p) return true;  return false;
}

template <class TKey>
typename bst<TKey>::iterator bst<TKey>::begin() {
  // temp node
  node *t = Troot;
  // if Troot is null return NULL iterator
  if (t == NULL) return iterator(t);
   
  while (t->left != NULL) {
    t = t->left;
  }
  return iterator(t);
}

template <class TKey>
typename bst<TKey>::iterator bst<TKey>::end() {
  // return iterator pointing to NULL
  iterator x; return x;
}

template <class TKey>
typename bst<TKey>::iterator bst<TKey>::lower_bound(const TKey &key) {
  node *t = Troot;
  node *n = NULL;
  while (t != NULL) {
    if (key <= t->key) {
      n = t;
      t = t->left;
    } else { t = t->right; }
  }
  return iterator(n);
}

template <class TKey>
typename bst<TKey>::iterator bst<TKey>::upper_bound(const TKey &key) {
  node *t = Troot;
  node *n = NULL;
  while (t != NULL) {
    if (key < t->key) {
      n = t;
      t = t->left;
    } else { t = t->right; }
  }
  return iterator(n);
}

template <class TKey>
void bst<TKey>::clear(node *T)
{
  if (T) {
    clear(T->left);
    clear(T->right);
    delete T;
    T = NULL;
  }
}

template <class TKey>
void bst<TKey>::insert(TKey &key)
{ 
  Troot = insert(Troot, key);
}

template <class TKey>
class bst<TKey>::node *bst<TKey>::insert(node *T, TKey &key)
{
  // set parent link below
  if (T == NULL) {
    // update and set node ID 
    // increment node ID counter
    T = new node(++I);
    T->key = key;
  } else if (T->key == key) {
    ;
  } else if (key < T->key) {
    T->left = insert(T->left, key);
    T->left->parent = T;
  } else {    
    T->right = insert(T->right, key);
    T->right->parent = T;
  }
  return T;
}

template <class TKey>
void bst<TKey>::print_bylevel()
{
  if (Troot == NULL)
    return;

  queue<node *> Q;
  node *T;

  Q.push(Troot);
  while (!Q.empty()) {
    T = Q.front();
    Q.pop();

    T->print();
    if (T->left)  Q.push(T->left);
    if (T->right) Q.push(T->right);
  }
}
#endif
