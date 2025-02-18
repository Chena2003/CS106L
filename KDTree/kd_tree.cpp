#include "kd_tree.h"
#include <stack>
#include <algorithm>
#include <unordered_map>

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() : 
      _root(nullptr),
      _size(0),
      _dimension(N) {

};

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
      clear(_root);
};

template <size_t N, typename ElemType> 
KDTree<N, ElemType>::KDTree(const KDTree& rhs) :
      _root(nullptr),
      _size(0),
      _dimension(rhs._dimension) {
      if(rhs._root == nullptr) 
            return ;

      std::stack<pointer> st;
      st.push(rhs._root);
      while(!st.empty()) {
            pointer t = st.top();
            st.pop();

            insert(t->point, t->element);

            if(t->left_node != nullptr)
                  st.push(t->left_node);
            if(t->right_node != nullptr)
                  st.push(t->right_node);
      }
};

template <size_t N, typename ElemType> 
KDTree<N, ElemType>& KDTree<N, ElemType>::operator=(const KDTree& rhs) {
      _dimension = rhs._dimension;
      _size = 0;

      if(rhs._root == nullptr) 
            return *this;

      std::stack<pointer> st;      
      st.push(rhs._root);
      while(!st.empty()) {
            pointer t = st.top();
            st.pop();

            insert(t->point, t->element);

            if(t->left_node != nullptr)
                  st.push(t->left_node);
            if(t->right_node != nullptr)
                  st.push(t->right_node);
      }

      return *this;
};

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
      return _dimension;
};

template <size_t N, typename ElemType> 
size_t KDTree<N, ElemType>::size() const {
      return _size;
};

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
      return _size == 0;
};

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
      return find_node(pt) != nullptr;
};

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value) {
      pointer curr = _root, prev = nullptr;
      size_t cnt = 0, cntt;
      double point1, point2;
      while(curr != nullptr && curr->point != pt) {
            cntt = cnt % _dimension;
            point1 = pt[cntt];
            point2 = curr->point[cntt];
            
            if(point1 < point2) {
                  prev = curr;
                  curr = curr->left_node;
            }
            else {
                  prev = curr;
                  curr = curr->right_node;
            }

            ++cnt;
      }      

      if(curr != nullptr) {
           curr->element = value; 
      }
      else if(prev != nullptr) {
            curr = new Node(pt, value);
            if(point1 < point2)
                  prev->left_node = curr;
            else 
                  prev->right_node = curr;
            ++_size;
      }
      else {
            _root = new Node(pt, value);
            ++_size;
      }
};

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[](const Point<N>& pt) {
      if(find_node(pt) == nullptr)
            insert(pt, ElemType{});

      return find_node(pt)->element;
};

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) {
      return const_cast<ElemType&>(static_cast<const KDTree<N, ElemType>*>(this)->at(pt));
};

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const {
      pointer t = find_node(pt);
      
      if(t == nullptr) 
            throw std::out_of_range("Index out of range.");
      return t->element;
};

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N>& key, size_t k) const {
      if(_root == nullptr)
            return ElemType{};
      
      BoundedPriorityQueue<ElemType> bpq(k);
      std::stack<std::pair<pointer, int>> st;

      st.push({_root, 0});
      while(!st.empty()) {
            auto [curr, cnt] = st.top();
            st.pop();

            bpq.enqueue(curr->element, Distance(key, curr->point));

            auto cntt = cnt % _dimension;
            auto point1 = key[cntt];
            auto point2 = (curr->point)[cntt];

            bool left = false, right = false; 
            if(point1 < point2) {
                  left = true;
                  if(curr->left_node != nullptr)
                        st.push({curr->left_node, cnt + 1});
            }
            else {
                  right = true;
                  if(curr->right_node != nullptr)
                        st.push({curr->right_node, cnt + 1});
            }

            if(bpq.size() < k || fabs(point2 - point1) < bpq.worst()) {
                  if(!left && curr->left_node != nullptr)
                        st.push({curr->left_node, cnt + 1});
                  if(!right && curr->right_node != nullptr)
                        st.push({curr->right_node, cnt + 1});
            }
      }

      std::unordered_map<ElemType, size_t> cnts;
      while(!bpq.empty()) {
            auto t = bpq.dequeueMin();
            ++cnts[t];            
      }

      ElemType e;
      size_t maxcnt = 0;
      for(const auto& cnt : cnts) {
            if(cnt.second > maxcnt) {
                  maxcnt = cnt.second;
                  e = cnt.first;
            }
      }

      return e;
};

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::clear(pointer t) {
      if(t == nullptr)
            return ;

      if(t->left_node == nullptr && t->right_node == nullptr) {
            delete t;
            return;
      }

      if(t->left_node != nullptr)
            clear(t->left_node);
      if(t->right_node != nullptr)
            clear(t->right_node);     
};

template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::pointer KDTree<N, ElemType>::find_node(const Point<N>& pt) const {
      pointer t = _root;
      size_t cnt = 0;
      while(t != nullptr && t->point != pt) {
            size_t cntt = cnt % _dimension;
            double point1 = pt[cntt];
            double point2 = t->point[cntt];
            
            if(point1 < point2) 
                  t = t->left_node;
            else 
                  t = t->right_node;

            ++cnt;
      }

      return t;
} 

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::debug() const {
      std::cout << "size = " << _size << " " \
                << "dimenion = " << _dimension << std::endl;

      if(_root == nullptr) 
          return ;

      std::cout.iword(std::ios_base::xalloc()) = 1;
      std::stack<std::pair<pointer, std::string>> st;
      st.push({_root, ""});
      while(!st.empty()) {
            auto [t, info] = st.top();
            st.pop();

            std::cout << std::setw(6) << std::left << (info.empty() ? "root" : info) << ": [ ";
            for(size_t i = 0; i < (t->point).size(); ++i) {
                  if(i == (info.size() % _dimension)) {
                        std::cout << "\033[31m" << std::setw(3) << (t->point)[i] << ' ' << "\033[0m";
                  }
                  else
                        std::cout << std::setw(3) << (t->point)[i] << ' ';
            }
            std::cout << ", " << t->element << " ]" << std::endl;

            if(t->left_node != nullptr)
                  st.push({t->left_node, info + "L"});
            if(t->right_node != nullptr)
                  st.push({t->right_node, info + "R"});
      }   

}
