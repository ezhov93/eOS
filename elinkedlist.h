#ifndef ELINKEDLIST_H
#define ELINKEDLIST_H

#include <stddef.h>

template <typename T> class ELinkedList {
public:
  ELinkedList();
  ~ELinkedList();

  int size();
  void append(int index, T value);
  void append(T value);
  T at(int index);
  void clear();
  void fill(T value);
  int indexOf(T value);
  inline void insert(int index, T value) { append(index, value); }
  T pop_back();
  T pop_front();
  inline void prepand(T value) { push_front(value); }
  inline void push_back(T value) { append(value); }
  void push_front(T value);
  void remove(int index);
  void setValue(int index, T value);
  void sort(int (*cmp)(T &, T &));
  inline T &operator[](int index);
  inline T &operator[](size_t &i) { return node(i)->data; }
  // inline const T& operator[](const size_t &i) const { return this->at(i); }

private:
  struct ListNode {
    T data;
    ListNode *next;
  };
  int _size;
  ListNode *_root;
  ListNode *_last;
  ListNode *_lastNodeGot;
  int _lastIndexGot;
  bool _cached;
  ListNode *node(int index);
  ListNode *findEndOfSortedString(ListNode *p, int (*cmp)(T &, T &));
};

template <typename T> ELinkedList<T>::ELinkedList() {
  _root = NULL;
  _last = NULL;
  _size = 0;

  _lastNodeGot = _root;
  _lastIndexGot = 0;
  _cached = false;
}

// Clear Nodes and free Memory
template <typename T> ELinkedList<T>::~ELinkedList() {
  ListNode *tmp;
  while (_root != NULL) {
    tmp = _root;
    _root = _root->next;
    delete tmp;
  }
  _last = NULL;
  _size = 0;
  _cached = false;
}

template <typename T>
typename ELinkedList<T>::ListNode *ELinkedList<T>::node(int index) {
  int _pos = 0;
  ListNode *current = _root;
  // Check if the node trying to at is
  // immediatly AFTER the previous got one
  if (_cached && _lastIndexGot <= index) {
    _pos = _lastIndexGot;
    current = _lastNodeGot;
  }
  while (_pos < index && current) {
    current = current->next;

    _pos++;
  }
  // Check if the object index got is the same as the required
  if (_pos == index) {
    _cached = true;
    _lastIndexGot = index;
    _lastNodeGot = current;
    return current;
  }
  return NULL;
}

template <typename T> int ELinkedList<T>::size() { return _size; }

template <typename T> void ELinkedList<T>::fill(T value) {
  for (int i = 0; i < size(); i++) {
    setValue(i, value);
  }
}
template <typename T> int ELinkedList<T>::indexOf(T value) {
  for (int i = 0; i < size(); ++i)
    if (at(i) == value)
      return i;
  return -1;
}

template <typename T> void ELinkedList<T>::append(int index, T value) {
  if (index >= _size)
    return append(value);
  if (index == 0)
    return push_front(value);

  ListNode *tmp = new ListNode(), *_prev = node(index - 1);
  tmp->data = value;
  tmp->next = _prev->next;
  _prev->next = tmp;

  _size++;
  _cached = false;
}

template <typename T> void ELinkedList<T>::append(T value) {
  ListNode *tmp = new ListNode();
  tmp->data = value;
  tmp->next = NULL;

  if (_root) {
    // Already have elements inserted
    _last->next = tmp;
    _last = tmp;
  } else {
    // First element being inserted
    _root = tmp;
    _last = tmp;
  }

  _size++;
  _cached = false;
}

template <typename T> void ELinkedList<T>::push_front(T value) {
  if (_size == 0)
    return append(value);

  ListNode *tmp = new ListNode();
  tmp->next = _root;
  tmp->data = value;
  _root = tmp;

  _size++;
  _cached = false;
}

template <typename T> T &ELinkedList<T>::operator[](int index) {
  return node(index)->data;
}

template <typename T> void ELinkedList<T>::setValue(int index, T value) {
  // Check if index position is in bounds
  if (index < 0 || index >= _size)
    return;
  node(index)->data = value;
}

template <typename T> T ELinkedList<T>::pop_back() {
  if (_size <= 0)
    return T();

  _cached = false;

  if (_size >= 2) {
    ListNode *tmp = node(_size - 2);
    T ret = tmp->next->data;
    delete (tmp->next);
    tmp->next = NULL;
    _last = tmp;
    _size--;
    return ret;
  } else {
    // Only one element left on the list
    T ret = _root->data;
    delete (_root);
    _root = NULL;
    _last = NULL;
    _size = 0;
    return ret;
  }
}

template <typename T> T ELinkedList<T>::pop_front() {
  if (_size <= 0)
    return T();

  if (_size > 1) {
    ListNode *_next = _root->next;
    T ret = _root->data;
    delete (_root);
    _root = _next;
    _size--;
    _cached = false;

    return ret;
  } else {
    // Only one left, then pop_back()
    return pop_back();
  }
}

template <typename T> void ELinkedList<T>::remove(int index) {
  if (index < 0 || index >= _size)
    return;

  if (index == 0)
    pop_front();
  else if (index == _size - 1)
    pop_back();

  else {

    ListNode *tmp = node(index - 1);
    ListNode *toDelete = tmp->next;
    T ret = toDelete->data;
    tmp->next = tmp->next->next;
    delete (toDelete);
    _size--;
    _cached = false;
  }
}

template <typename T> T ELinkedList<T>::at(int index) {
  ListNode *tmp = node(index);
  return (tmp ? tmp->data : T());
}

template <typename T> void ELinkedList<T>::clear() {
  while (size() > 0)
    pop_front();
}

template <typename T> void ELinkedList<T>::sort(int (*cmp)(T &, T &)) {
  if (_size < 2)
    return;

  for (;;) {

    ListNode **joinPoint = &_root;

    while (*joinPoint) {
      ListNode *a = *joinPoint;
      ListNode *a_end = findEndOfSortedString(a, cmp);

      if (!a_end->next) {
        if (joinPoint == &_root) {
          _last = a_end;
          _cached = false;
          return;
        } else {
          break;
        }
      }

      ListNode *b = a_end->next;
      ListNode *b_end = findEndOfSortedString(b, cmp);

      ListNode *tail = b_end->next;

      a_end->next = NULL;
      b_end->next = NULL;

      while (a && b) {
        if (cmp(a->data, b->data) <= 0) {
          *joinPoint = a;
          joinPoint = &a->next;
          a = a->next;
        } else {
          *joinPoint = b;
          joinPoint = &b->next;
          b = b->next;
        }
      }

      if (a) {
        *joinPoint = a;
        while (a->next)
          a = a->next;
        a->next = tail;
        joinPoint = &a->next;
      } else {
        *joinPoint = b;
        while (b->next)
          b = b->next;
        b->next = tail;
        joinPoint = &b->next;
      }
    }
  }
}

template <typename T>
typename ELinkedList<T>::ListNode *
ELinkedList<T>::findEndOfSortedString(ListNode *p, int (*cmp)(T &, T &)) {
  while (p->next && cmp(p->data, p->next->data) <= 0) {
    p = p->next;
  }
  return p;
}

#endif // ELINKEDLIST_H
