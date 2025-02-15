#include "class.h"
#include <algorithm>

template <typename T>
MyVector<T>::MyVector() noexcept : 
      logicalSize(0), 
      allocatedSize(kDefaultSize),
      _item(new T[allocatedSize])
{

};

template <typename T>
MyVector<T>::MyVector(size_type count, const value_type& val) noexcept : 
      logicalSize(count),
      allocatedSize(std::max(kDefaultSize, 2 * count)),
      _item(new T[allocatedSize])
{
      std::fill(begin(), end(), val);
};

template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> args) noexcept : 
      logicalSize(0),
      allocatedSize(std::max(kDefaultSize, 2 * args.size())),
      _item(new T[allocatedSize])
{
      for(auto arg : args) {
            push_back(arg);
      }
};

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other) noexcept : 
      logicalSize(other.logicalSize),
      allocatedSize(other.allocatedSize),
      _item(new T[allocatedSize])
{
      std::copy(other.begin(), other.end(), _item);
};

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& rhs) noexcept {
      if(this != &rhs) {
            delete _item;

            logicalSize = rhs.logicalSize;
            allocatedSize = rhs.allocatedSize;
            _item = new T[allocatedSize];

            std::copy(rhs.begin(), rhs.end(), _item);
      }

      return *this;
};

template <typename T>
MyVector<T>::MyVector(MyVector<T>&& rhs) noexcept :
      logicalSize(std::move(rhs.logicalSize)),
      allocatedSize(std::move(rhs.allocatedSize)),
      _item(std::move(rhs._item))
{
      logicalSize = std::move(rhs.logicalSize);
      allocatedSize = std::move(rhs.allocatedSize);
      _item = std::move(rhs._item);
      rhs._item = nullptr;
};

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& rhs) noexcept {
      if(this != &rhs) {
            delete _item;

            logicalSize = std::move(rhs.logicalSize);
            allocatedSize = std::move(rhs.allocatedSize);
            _item = std::move(rhs._item);

            rhs._item = nullptr;            
      }
};

template <typename T>
MyVector<T>::~MyVector() noexcept {
      delete _item;
};

template <typename T>
typename MyVector<T>::reference MyVector<T>::at(size_type n) {
    return const_cast<value_type&>(static_cast<const MyVector<T>*>(this)->at(n));
}

template <typename T>
typename MyVector<T>::const_reference MyVector<T>::at(size_type n) const {
    if (n >= allocatedSize) {
        throw std::out_of_range("Out of bounds access of vector");
    }
    return _item[n];
}

template <typename T>
typename MyVector<T>::reference MyVector<T>::operator[](size_type n) {
    return const_cast<value_type&>(
                static_cast<const MyVector<T>*>(this)->operator[](n));
}

template <typename T>
typename MyVector<T>::const_reference MyVector<T>::operator[](size_type n) const {
    return _item[n];
}

template <typename T>
typename MyVector<T>::Iterator MyVector<T>::insert(Iterator it, const value_type& elem) {
      size_type index = it - begin();
      if(logicalSize == allocatedSize) {
            reserve(2 * allocatedSize);
      }
      
      Iterator pos = begin() + index;
      std::copy_backward(pos, end(), end() + 1);
      *it = elem;
      ++logicalSize;

      return it;
}

template <typename T>
void MyVector<T>::push_back(const value_type& elem) {
      insert(end(), elem);
};

template <typename T>
void MyVector<T>::reserve(size_type size) {
      if(size > allocatedSize) {
            allocatedSize = size;

            Iterator newitem = new T[allocatedSize];

            std::copy(begin(), end(), newitem);

            delete _item;
            _item = std::move(newitem);            
      }
}
template <typename T>
void MyVector<T>::privatefunc() {
            std::cout << "This is a praivate function." << std::endl;
};
template <typename T>
typename MyVector<T>::size_type MyVector<T>::_size() const {
      return _item[1];
}

template <typename T>
void MyVector<T>::set_size(size_type size) {
      _item[1] = size;
}