/*
 * CS106L Assignment 5: TreeBook
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include <iostream>
#include <string>
#include <algorithm>

class User
{
public:
  User(const std::string& name);
  void add_friend(const std::string& name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string& name);

  User(const User& user) : 
        _name(user._name),
        _size(user._size),
        _capacity(user._capacity),
        _friends(new std::string [_capacity])
  {
    std::copy(user._friends, user._friends + _size, _friends);
  }

  User& operator=(const User& user) {
    if(this != &user) {
      delete [] _friends;
      _name = user._name;
      _size = user._size;
      _capacity = user._capacity;
      _friends = new std::string [_capacity];

      std::copy(user._friends, user._friends + _size, _friends);
    }    

    return *this;
  }

  User(User&& user) = delete;
  User& operator=(User&& user) = delete;

  ~User() {
    delete[] _friends;
  };

  friend std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "User(name=" << user._name << ", friends=[";
    for(int i = 0; i < user._size; ++i) os << (!i ? "" : ", ") << user._friends[i]; 
    os << "])";

    return os;
  };

  User& operator+=(const User& user) {
    add_friend(user._name);
    // user.add_friend(_name);

    const_cast<User&>(user).add_friend(_name);

    return *this;
  };

  bool operator<(const User& user) const {
    return _name < user._name;
  };

private:
  std::string _name;
  size_t _size;
  size_t _capacity;
  std::string* _friends;
};