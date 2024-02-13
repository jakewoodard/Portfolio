#ifndef SLIST_H
#define SLIST_H

// include header file(s) needed

// qsort_r3: randomly chosen pivot, 3-way partition {<},{=},{>}

// template <typename T>
template <class T>
class slist {
  private:
    struct node {
      node() { data = T(); next = NULL; }
      node(const T &key) { data = key; next = NULL; }
      // add overloaded operator< code
      bool operator<(const node &j) { return data < j.data; }  
      T data;
      node *next;
    };

   // smart pointer class
   class sptr {
     public:
       sptr(node *_ptr = NULL) { ptr = _ptr; }
       bool operator< (const sptr & rhs) const {
         return *ptr < *rhs.ptr;
       }
                
       operator node * () const { return ptr; }

     private:
       node * ptr;
    };
 
  public:
    class iterator {
      public:
        iterator() { p = NULL; }
        T & operator*() { return p->data; }
        iterator & operator++() { p = p->next; return *this; }
        bool operator!=(const iterator & rhs) const { return p != rhs.p; }

      private:
        iterator(node *n_p) { p = n_p; }
        node *p;

      friend class slist<T>;
    };

  public:
    slist();
    ~slist();

    void push_back(const T &);
    void sort(const string &);

    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(NULL); }

  private:
    node *head;
    node *tail;
};

template <typename T>
slist<T>::slist() {
  head = new node();
  tail = head;
}

template <typename T>
slist<T>::~slist() {
  while (head->next != NULL) {
    node *p = head->next;
    head->next = p->next;
    delete p;
  }
  delete head;

  head = NULL;
  tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
  tail->next = new node(din);
  tail = tail->next;
}

template <typename T>
void slist<T>::sort(const string &algname) {
  // determine number of list elements
  node *p = head->next;
  int N = 0;
  while (p != NULL) { N++; p = p->next; } 
  
  // set up smart pointer array called Ap
  vector<sptr> Ap(N);
  node *k = head->next;
  for (int i = 0; i < N; i++) {
    Ap[i] = k;
    k = k->next; 
  } 

  if (algname == "-quicksort") {
    std::sort(Ap.begin(), Ap.end());
  }

  if (algname == "-mergesort") {
    std::stable_sort(Ap.begin(), Ap.end());
  }

  // if qsort_r3, apply qsort_r3(...)

  // use sorted Ap array to relink list 
  node *t = head; 
  for (int i = 0; i < N; i++) {
    t->next = Ap[i];
    t->next->next = NULL;
    t = t->next;
  } 
}

#endif // SLIST_H
