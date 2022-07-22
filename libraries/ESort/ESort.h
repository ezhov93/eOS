#ifndef ESORT_H
#define ESORT_H

template <typename T>
void swap(T &a, T &b) {
  T tmp = a;
  a = b;
  b = tmp;
}

namespace ESort {
template <typename T>
void bubble(T array[], int size) {
  for (int i = 0; i < size - 1; ++i)
    for (int j = 0; j < size - i - 1; ++j)
      if (array[j] > array[j + 1]) swap(array[j], array[j + 1]);
}

template <typename T>
void insertion(T array[], int size) {
  T key;
  for (int i = 1; i < size; ++i) {
    key = array[i];
    int j = i - 1;
    while (j >= 0 && array[j] > key) {
      array[j + 1] = array[j];
      --j;
    }
    array[j + 1] = key;
  }
}

template <typename T>
void selection(T array[], int size) {
  for (int i = 0; i < size - 1; ++i) {
    int min_idx = i;
    for (int j = i + 1; j < size; ++j)
      if (array[j] < array[min_idx]) min_idx = j;
    swap<T>(array[min_idx], array[i]);
  }
}

template <typename T>
int partition(T array[], int size) {
  int pivot = array[size];
  int i = -1;
  for (int j = 0; j <= size - 1; j++)
    if (array[j] <= pivot) swap<T>(array[++i], array[j]);
  swap<T>(array[i + 1], array[size]);
  return i + 1;
}

template <typename T>
void quickSort(T array[], int size) {
  if (array < &array[size]) {
    int pivot = partition<T>(array, size);
    quickSort<T>(array, pivot - 1);
    quickSort<T>(array[pivot + 1], size);
  }
}

}  // namespace ESort

#endif  // ESORT_H


