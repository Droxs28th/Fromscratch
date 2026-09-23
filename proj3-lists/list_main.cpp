#include <iostream>

#include "circvector.h"
#include "linkedlist.h"

using namespace std;

int main() {
  CircVector<int> circVec(5);
  circVec.push_back(10);
  circVec.push_back(20);
  circVec.push_back(30);

  cout << "CircVector size: " << circVec.size() << endl;

  LinkedList<int> linkedList;
  linkedList.push_back(100);
  linkedList.push_back(200);
  linkedList.push_back(300);

  cout << "LinkedList size: " << linkedList.size() << endl;
  cout << "LinkedList first element: " << linkedList.front() << endl;

  return 0;
}
