#ifndef __CLASS_H__
#define __CLASS_H__

#include <iostream>


template <typename T>
class MyVector {
      using value_type = T;
      using Iterator = T*;
      using const_iterator = const T*;
      using reference = T&;
      using const_reference = const T&;
      using size_type = size_t;

      const size_type kDefaultSize = 10;

public:
      MyVector() noexcept;
      MyVector(size_type count, const value_type& val) noexcept;
      MyVector(std::initializer_list<T> args) noexcept;
      MyVector(const MyVector<T>& other) noexcept;

      MyVector<T>& operator=(const MyVector<T>& rhs) noexcept;

      MyVector(MyVector<T>&& rhs) noexcept;
      MyVector<T>& operator=(MyVector<T>&& rhs) noexcept;
      ~MyVector() noexcept;

      size_type size() const { return logicalSize; };
      bool empty() const { return logicalSize == 0; };

      reference operator[](size_type index);
      const_reference operator[](size_type index) const;

      reference at(size_type index);
      const_reference at(size_type index) const;

      Iterator insert(Iterator it, const value_type& elem);
      void push_back(const value_type& elem);

      void reserve(size_type size);

      Iterator begin() { return _item; };
      Iterator end() { return _item + logicalSize; };

      size_type _size() const;
      void set_size(size_type size);

private:
      void privatefunc();


      size_type logicalSize;
      size_type allocatedSize;
      Iterator _item;
};

#include "class.cpp"

#endif // __CLASS_H__