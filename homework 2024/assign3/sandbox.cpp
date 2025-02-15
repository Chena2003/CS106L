/*
 * CS106L Assignment 3: Make a Class
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */
#include "class.h"
#include <iostream>

void sandbox() {
  MyVector<int> Vec1;
  MyVector<int> Vec2(10, 1);
  MyVector<int> Vec3{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

  for(auto i : Vec3) {
    std::cout << i << ' ';
  }

}