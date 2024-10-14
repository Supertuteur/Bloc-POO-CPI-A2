#include <iostream>

using namespace std;

typedef int ArrayElement;

string array_element_to_string(ArrayElement *el) {
  return to_string(*el);
}

class DynamicArray {
private:
  int count;
  int size;

  ArrayElement **els;

  const float REFIT_THRESHOLD = 0.8;
  const float RESIZE_THRESHOLD = 0.8;

private:
  void resize(int size) {
    try {
      ArrayElement **temp;

      this->size = size;
      temp = new ArrayElement *[size];
      for (int i = 0; i < count; i++) {
        temp[i] = els[i];
      }

      delete[] els;
      els = temp;
    } catch (std::bad_alloc e) {
      size = size / 2;
    }
  }
  void fit() { resize(count); }

public:
  DynamicArray(int size = 1) {
    count = 0;
    els = new ArrayElement *[size];
  }
  int add(ArrayElement *el) {
    if (count >= RESIZE_THRESHOLD * size) {
      resize(size * 2);
    }

    els[count] = el;

    return count++;
  }
  ArrayElement *remove(int i) {
    ArrayElement *ptr = nullptr;

    if(0 <= i < count) {
      ptr = els[i];
      els[i] = els[--count];

      if (count < REFIT_THRESHOLD * size) {
        fit();
      }
    }
    
    return ptr;
  }
  ArrayElement *get(int i) { return els[i]; }
  void print() {
    cout << "[";
    if(count > 0) {
      for(int i = 0; i < count - 1; i++) {
        cout << array_element_to_string(els[i]) << ", ";
      }
      cout << array_element_to_string(els[count - 1]);
    }
    cout << "]";
  }
};

int main() {
  DynamicArray arr;

  int a = 2, b = 3, c = 5;

  arr.add(&a);
  arr.add(&b);
  arr.add(&c);

  arr.remove(1);

  arr.print();

  return 0;
}