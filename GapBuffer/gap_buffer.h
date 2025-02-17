
#ifndef GAPBUFFER_H
#define GAPBUFFER_H
#include <vector>
#include <iostream> // for cout in debug
#include <algorithm>
#include <sstream> // for stringstreams
#include <memory> // for unique_ptr
#include <cstdlib>

const int kDefaultSize = 10;

// forward declaration for the GapBufferIterator class
template <typename T>
class GapBufferIterator;

// declaration for the GapBuffer class
template <typename T>
class GapBuffer {
public:
    friend class GapBufferIterator<T>;

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using iterator = GapBufferIterator<T>;

    explicit GapBuffer();
    explicit GapBuffer(size_type count, const value_type& val = value_type());
    ~GapBuffer();
    GapBuffer(std::initializer_list<T> init);
    GapBuffer(const GapBuffer& other);
    GapBuffer(GapBuffer&& other);
    GapBuffer& operator=(const GapBuffer& rhs);
    GapBuffer& operator=(GapBuffer&& rhs);

    void insert_at_cursor(const_reference element);
    void insert_at_cursor(value_type&& element);
    template <typename... Args>
    void emplace_at_cursor(Args&&... args); // optional
    void delete_at_cursor();
    reference get_at_cursor();
    const_reference get_at_cursor() const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    void move_cursor(int num);
    void reserve(size_type new_size);
    size_type size() const;
    size_type cursor_index() const;
    bool empty() const;
    void debug() const;

    iterator begin();
    iterator end();
    iterator cursor();  

private:
    size_type _logical_size; // uses external_index
    size_type _buffer_size;  // uses array_index
    size_type _cursor_index; // uses array_index
    size_type _gap_size;
    pointer _elems;          // uses array_index

    size_type to_external_index(size_type array_index) const;
    size_type to_array_index(size_type external_index) const;
    void move_to_left_of_buffer(size_type num);
};

// Class declaration of the GapBufferIterator class
template <typename T>
class GapBufferIterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
    friend class GapBuffer<T>;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using iterator = GapBufferIterator<T>;

    reference operator*();
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);

    // the operators below are implemented as non-friend non-members
    // iterator operator+(const iterator& lhs, size_type diff);
    // iterator operator-(const iterator& lhs, size_type diff);
    // iterator operator+(size_type diff, const iterator& rhs);

    /* we implemented these for you. don't change these */
    friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs._index == rhs._index; }
    friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
    friend difference_type operator-(const iterator& lhs, const iterator& rhs) { return lhs._index - rhs._index; }
    iterator& operator+=(size_type diff) { _index += diff; return *this; }
    iterator& operator-=(size_type diff) { _index -= diff; return *this; }
    friend bool operator>(const iterator& lhs, const iterator& rhs)  { return lhs._index < rhs._index; }
    friend bool operator<(const iterator& lhs, const iterator& rhs)  { return lhs._index > rhs._index; }
    friend bool operator<=(const iterator& lhs, const iterator& rhs)  { return !(lhs > rhs); }
    friend bool operator>=(const iterator& lhs, const iterator& rhs)  { return !(lhs < rhs); }
    reference operator[](size_t index) { return *(*this + index); }

    friend std::ostream& operator<< (std::ostream& os, const iterator& It) {
        std::cout << "Index: " << It._index << std::endl;

        return os;
    }

private:
    GapBuffer<T>* _pointee;
    size_t _index;
    GapBufferIterator(GapBuffer<T>* pointee, size_t index) : _pointee(pointee), _index(index) {}
};

// Part 1: basic functions
template <typename T>
GapBuffer<T>::GapBuffer() : 
    _logical_size(0),
    _buffer_size(kDefaultSize),
    _cursor_index(0),
    _gap_size(kDefaultSize),
    _elems(new T[_buffer_size]) { }

template <typename T>
GapBuffer<T>::GapBuffer(size_type count, const value_type& val) : 
    _logical_size(count),
    _buffer_size(std::max(static_cast<size_type>(kDefaultSize), 2 * count)),
    _cursor_index(count),
    _gap_size(_buffer_size - _logical_size),
    _elems(new T[_buffer_size]) { 
    std::fill(_elems, _elems + _logical_size, val);
}

template <typename T>
void GapBuffer<T>::insert_at_cursor(const_reference element) {
    if(_logical_size == _buffer_size) 
        reserve(2 * _buffer_size);

    _elems[_cursor_index] = element;
    ++_cursor_index;
    ++_logical_size;
    --_gap_size;
}

template <typename T>
void GapBuffer<T>::delete_at_cursor() {
    if(!_cursor_index)
        // throw std::out_of_range("cursor index = 0.");
        return ;

    --_cursor_index;
    --_logical_size;
    ++_gap_size;
}

template <typename T>
typename GapBuffer<T>::reference GapBuffer<T>::get_at_cursor() {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick here after implementing the const-version.

    return const_cast<reference>(static_cast<const GapBuffer<T>* >(this)->get_at_cursor());
}

template <typename T>
typename GapBuffer<T>::reference GapBuffer<T>::at(size_type pos) {
    // TODO: implement this function (~1 line long)
    // Hint: at should do error-checking!

    return const_cast<reference>(static_cast<const GapBuffer<T>* >(this)->at(pos));
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::size() const {
    // TODO: implement this function (~1 line long)
    return _logical_size;
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::cursor_index() const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide

    // return to_external_index(_cursor_index); // _cursor_index 本身就是 array index
    return _cursor_index;
}

template <typename T>
bool GapBuffer<T>::empty() const {
    // TODO: implement this function (~1 line long)

    return _logical_size == 0;
}

// Part 2: const-correctness

template <typename T>
typename GapBuffer<T>::const_reference GapBuffer<T>::get_at_cursor() const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick in the non-const version.

    return _elems[_cursor_index];
}

template <typename T>
typename GapBuffer<T> ::const_reference GapBuffer<T>::at(size_type pos) const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick in the non-const version.
        
    if(pos >= _logical_size)
        throw std::out_of_range("Index out of range.");

    return _elems[to_array_index(pos)];
}

// Part 3: operator overloading
template <typename T>
typename GapBuffer<T>::reference GapBuffer<T>::operator[](size_type pos) {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick here after implementing the const-version.

    return const_cast<reference>(static_cast<const GapBuffer<T> *>(this)->operator[](pos));
}

template <typename T>
typename GapBuffer<T>::const_reference GapBuffer<T>::operator[](size_type pos) const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick in the non-const version.

    return _elems[to_array_index(pos)];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const GapBuffer<T>& buf) {
    std::ostringstream oss;
    auto& buf_nonconst = const_cast<GapBuffer<T>&>(buf); // 必须用引用避免拷贝
    auto pos = buf_nonconst.cursor();
    bool first = true;

    oss << "{";
    for (auto it = buf_nonconst.begin(); it != buf_nonconst.end(); ++it) {
        if (!first) oss << ", ";
        first = false;

        if (it == pos) 
            oss << "^";

        oss << *it;
    }

    // 处理光标在末尾的情况
    if (pos == buf_nonconst.end()) oss << "^";

    oss << "}";

    os << oss.str();
    return os;
}

template <typename T>
bool operator==(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    // Hint: std::equal can be used after you implement iterators

    // if(lhs.size() != rhs.size())
    //     return false;

    // for(size_t i = 0; i < lhs.size(); ++i) 
    //     if(lhs[i] != rhs[i])
    //         return false;

    // return true;
    auto& lhs_nonconst = const_cast<GapBuffer<T>&>(lhs);
    auto& rhs_nonconst = const_cast<GapBuffer<T>&>(rhs);    
    return std::equal(lhs_nonconst.begin(), lhs_nonconst.end(), rhs_nonconst.begin(), rhs_nonconst.end());
}

template <typename T>
bool operator!=(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    // Hint: how are == and != related?
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~3 lines long)
    // Hint: std::lexicographical_compare can be used after you implement iterators.

    // Hint: if you get warnings about const_iterator, you can try using const_cast
    // as a hack to cast away the const-ness of your parameter.
    // auto& lhs_nonconst = const_cast<GapBuffer<T>&>(lhs);
    // auto& rhs_nonconst = const_cast<GapBuffer<T>&>(rhs);
    // use lhs_nonconst.begin(), etc.

    // for(size_t i = 0; i < lhs.size() && i < rhs.size(); ++i) 
    //     if(lhs[i] != rhs[i])
    //         return lhs[i] < rhs[i];

    // return lhs.size() < rhs.size();

    auto& lhs_nonconst = const_cast<GapBuffer<T>&>(lhs);
    auto& rhs_nonconst = const_cast<GapBuffer<T>&>(rhs);
    return std::lexicographical_compare(lhs_nonconst.begin(), lhs_nonconst.end(), rhs_nonconst.begin(), rhs_nonconst.end());
}

template <typename T>
bool operator>(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    return lhs != rhs && !(lhs < rhs);
}

template <typename T>
bool operator<=(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)

    return !(lhs > rhs);
}

template <typename T>
bool operator>=(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)

    return !(lhs < rhs);
}

// Part 4: turn everything into a template!


// Part 5: Implement iterators
template <typename T>
typename GapBufferIterator<T>::reference GapBufferIterator<T>::operator*() {
    // TODO: implement this operator (~1 line long)
    return _pointee->at(_index);
}

template <typename T>
GapBufferIterator<T>& GapBufferIterator<T>::operator++() {
    // TODO: implement this prefix operator (~2 lines long)
    ++_index;    
    return *this;
}

template <typename T>
GapBufferIterator<T> GapBufferIterator<T>::operator++(int) {
    // TODO: implement this postfix operator (~3 lines long)
    auto t = *this;
    ++_index;
    return t;
}

template <typename T>
GapBufferIterator<T>& GapBufferIterator<T>::operator--() {
    // TODO: implement this prefix operator (~2 lines long)
    --_index;
    return *this;
}

template <typename T>
GapBufferIterator<T> GapBufferIterator<T>::operator--(int) {
    // TODO: implement this postfix operator (~3 lines long)
    auto t = *this;
    --_index;
    return t;
}

template <typename T>
GapBufferIterator<T> operator+(const GapBufferIterator<T>& lhs,
                               typename GapBufferIterator<T>::size_type diff) {
    // TODO: implement this operator (~3 lines long)
    // Note: this operator is not a friend of the GapBufferIterator class
    // Hint: write the operator in terms of +=

    auto t = lhs;
    t += diff;

    return t;
}

template <typename T>
GapBufferIterator<T> operator+(typename GapBufferIterator<T>::size_type diff,
                               const GapBufferIterator<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    // Note: this operator is not a friend of the GapBufferIterator class
    // Hint: write the operator in terms of the operator+ you wrote above.

    return rhs + diff;
}

template <typename T>
GapBufferIterator<T> operator-(const GapBufferIterator<T>& lhs,
                               typename GapBufferIterator<T>::size_type diff) {
    // TODO: implement this operator (~3 lines long)
    // Note: this operator is not a friend of the GapBufferIterator class
    // Hint: write the operator in terms of -=
    auto t = lhs;
    t -= diff;

    return t;
}

// The functions that are part of the GapBuffer class is provided for you!
template <typename T>
typename GapBuffer<T>::iterator GapBuffer<T>::begin() {
    return iterator(this, 0);
}

template <typename T>
typename GapBuffer<T>::iterator GapBuffer<T>::end() {
    return iterator(this, _logical_size);
}

template <typename T>
typename GapBuffer<T>::iterator GapBuffer<T>::cursor() {
    return iterator(this, _cursor_index);
}

// Part 6: Constructors and assignment

template <typename T>
GapBuffer<T>::~GapBuffer() {
    delete[] _elems;
}
template <typename T>
GapBuffer<T>::GapBuffer(std::initializer_list<T> init) : 
    GapBuffer(init.size()) {
    // TODO: implement this initializer list constructor (~2 lines long)
    std::copy(init.begin(), init.end(), _elems);
}

template <typename T>
GapBuffer<T>::GapBuffer(const GapBuffer& other) :
    _logical_size(other._logical_size),
    _buffer_size(other._buffer_size),
    _cursor_index(other._cursor_index),
    _elems(new T[_buffer_size])
{
    // TODO: implement this copy constructor (~4 lines long)
    // use member initialization list!
    std::copy(other._elems, other._elems + other._buffer_size, _elems);
}

template <typename T>
GapBuffer<T>& GapBuffer<T>::operator=(const GapBuffer& rhs) {
    if(this != &rhs) {
        delete[] _elems;
        _logical_size = rhs._logical_size;
        _buffer_size = rhs._buffer_size;
        _cursor_index = rhs._cursor_index;

        _elems = new T[_buffer_size];
        std::copy(rhs._elems, rhs._elems + rhs._buffer_size, _elems);
    }

    return *this;
}

// Part 7: Move semantics
template <typename T>
GapBuffer<T>::GapBuffer(GapBuffer&& other) :
    _logical_size(std::move(other._logical_size)),
    _buffer_size(std::move(other._buffer_size)),
    _cursor_index(std::move(other._cursor_index)),
    _elems(std::move(other._elems)) {
    // TODO: implement this move constructor (~4 lines long)
    // use initializer list!

    // Hint: if you get warnings about const_iterator, you can try using const_cast
    // as a hack to cast away the const-ness of your parameter.
    // auto& other_nonconst = const_cast<GapBuffer<T>&>(other);
    // use other.begin(), etc.
    other._elems = nullptr;
}

template <typename T>
GapBuffer<T>& GapBuffer<T>::operator=(GapBuffer&& rhs) {
    // TODO: implement this move assignment operator (~7 lines long)

    // Hint: if you get warnings about const_iterator, you can try using const_cast
    // as a hack to cast away the const-ness of your parameter.
    // auto& rhs_nonconst = const_cast<GapBuffer<T>&>(rhs);
    // use rhs.begin(), etc.

    if(this != &rhs) {
        delete[] _elems;

        _logical_size = std::move(rhs._logical_size);
        _buffer_size = std::move(rhs._buffer_size);
        _cursor_index = std::move(rhs._cursor_index);
        _elems = std::move(rhs._elems);
        rhs._elems = nullptr;
    }

    return *this;
}

template <typename T>
void GapBuffer<T>::insert_at_cursor(value_type&& element) {
    // TODO: implement this insert function (takes in an r-value) (~7 lines long)
    // insert_at_cursor(element); // by default, calls the l-value version above
    // when you are ready to implement, remove the insert_at_cursor call.

    if(_logical_size == _buffer_size) 
        reserve(2 * _logical_size);

    _elems[_cursor_index] = std::move(element);
    ++_cursor_index;
    ++_logical_size;
    --_gap_size;    
}

// Part 8: Make your code RAII-compliant - change the code throughout

// optional:
template <typename T>
template <typename... Args>
void GapBuffer<T>::emplace_at_cursor(Args&&... args) {
    // TODO: optional: implement function
    // remember to perfectly forward the arguments to the constructor of T.
    insert_at_cursor(T(std::forward<Args>(args)...));
}


// We've implemented the following functions for you.
// However...they do use raw pointers, so you might want to turn them into smart pointers!
template <typename T>
void GapBuffer<T>::move_cursor(int delta) {
    int new_index = _cursor_index + delta;
    if (new_index < 0 || new_index > static_cast<int>(_buffer_size)) {
        throw std::string("move_cursor: delta moves cursor out of bounds");
    }
    if (delta > 0) {
        auto begin_move = _elems + _cursor_index + _gap_size;
        auto end_move = begin_move + delta;
        auto destination = _elems + _cursor_index;
        std::move(begin_move, end_move, destination);
    } else {
        auto end_move = _elems + _cursor_index;
        auto begin_move = end_move + delta;
        auto* destination = _elems + _cursor_index + _gap_size + delta;
        std::move(begin_move, end_move, destination);
    }
    _cursor_index += delta;
}

template <typename T>
void GapBuffer<T>::reserve(size_type new_size) {
    if (_logical_size >= new_size) return;
    auto new_elems = new T[new_size];
    std::move(_elems, _elems + _cursor_index, new_elems);
    size_t new_gap_size = new_size - _logical_size;
    std::move(_elems + _buffer_size - _logical_size + _cursor_index,
              _elems + _buffer_size,
              new_elems + _cursor_index + new_gap_size);
    _buffer_size = new_size;
    delete [] _elems;
    _elems = std::move(new_elems);
    _gap_size = new_gap_size;
}

template <typename T>
void GapBuffer<T>::debug() const {
    std::cout << "logical size = " << _logical_size << " buffer size = " << _buffer_size << " cursor index = " << _cursor_index << std::endl;
    std::cout << "[";
    for (size_t i = 0; i < _buffer_size; ++i) {
        if (i == _cursor_index) {
            std::cout << "|";
        } else {
            std::cout << " ";
        }
        if (i >= _cursor_index && i < _cursor_index + _gap_size) {
            std::cout << "*";
        } else {
            std::cout << _elems[i];
        }
    }
    std::cout << (_cursor_index == _buffer_size ? "|" : " ");
    std::cout << "]" << std::endl;
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::to_external_index(size_type array_index) const {
    if (array_index <= _cursor_index) {
        return array_index;
    } else if (array_index >= _cursor_index + _gap_size){
        return array_index - _cursor_index;
    } else {
        throw std::out_of_range("to_external_index: array_index is out of bounds!");
    }
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::to_array_index(size_type external_index) const {
    if (external_index <= _cursor_index) {
        return external_index;
    } else {
        return external_index + _gap_size;
    }
}


#endif // GAPBUFFER_H
