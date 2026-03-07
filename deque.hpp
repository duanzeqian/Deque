#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

template<class T>
class double_list {
private:
	struct Node
	{
		T data;
		Node* prev;
		Node* next;
		Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
	};

	Node* head;
	Node* tail;
	size_t size;
public:
	/**
	 * elements
	 * add whatever you want
	 */

	// --------------------------
	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	 */
	double_list() : head(nullptr), tail(nullptr), size(0) {}
	double_list(const double_list<T> &other) : head(nullptr), tail(nullptr), size(0)
	{
		Node* pos = other.head;
		while (pos != nullptr)
		{
			insert_tail(pos->data);
			pos = pos->next;
		}
	}
	~double_list() {
		clear();
	}
	double_list& operator=(const double_list& other)
	{
		if (this != &other)
		{
			clear();
			Node* pos = other.head;
			while (pos != nullptr)
			{
				insert_tail(pos->data);
				pos = pos->next;
			}
		}
		return *this;
	}

	class iterator {
	private:
		friend class double_list;
		Node* iter;
		double_list* list;
	public:
		/**
		 * elements
		 * add whatever you want
		 */
		// --------------------------
		/**
		 * the follows are constructors and destructors
		 * you can also add some if needed.
		 */
		iterator(Node* it = nullptr, double_list* l = nullptr) : iter(it), list(l) {}
		iterator(const iterator &t) : iter(t.iter), list(t.list) {}
		~iterator() = default;
		
		/**
		 * iter++
		 */
		iterator operator++(int) {
			if (iter == nullptr) throw("invalid");
			iterator tmp = *this;
			iter = iter->next;
			return tmp;
		}
		/**
		 * ++iter
		 */
		iterator &operator++() {
			if (iter == nullptr) throw("invalid");
			iter = iter->next;
			return *this;
		}
		/**
		 * iter--
		 */
		iterator operator--(int) {
			if (iter == nullptr)
			{
				if (list == nullptr || list->tail == nullptr) throw("invalid");
				iterator tmp = *this;
				iter = list->tail;
				return tmp;
			}
			else
			{
				if (iter->prev == nullptr) throw("invalid");
				iterator tmp = *this;
				iter = iter->prev;
				return tmp;
			}
		}
		/**
		 * --iter
		 */
		iterator &operator--() {
			if (iter == nullptr)
			{
				if (list == nullptr || list->tail == nullptr) throw("invalid");
				iter = list->tail;
			}
			else
			{
				if (iter->prev == nullptr) throw("invalid");
				iter = iter->prev;
			}
			return *this;
		}
		
		/**
		 * if the iter didn't point to a value
		 * throw " invalid"
		 */
		T &operator*() const {
			if (iter == nullptr) throw("invalid");
			return iter->data;
		}
		
		/**
		 * other operation
		 */
		T *operator->() const noexcept {
			return &(iter->data);
		}
		bool operator==(const iterator &rhs) const {
			return iter == rhs.iter && list == rhs.list;
		}
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	/**
	 * return an iterator to the beginning
	 */
	iterator begin() {
		return iterator(head, this);
	}
	/**
	 * return an iterator to the ending
	 * in fact, it returns the iterator point to nothing,
	 * just after the last element.
	 */
	iterator end() {
		return iterator(nullptr, this);
	}
	/**
	 * if the iter didn't point to anything, do nothing,
	 * otherwise, delete the element pointed by the iter
	 * and return the iterator point at the same "index"
	 * e.g.
	 * 	if the origin iterator point at the 2nd element
	 * 	the returned iterator also point at the
	 *  2nd element of the list after the operation
	 *  or nothing if the list after the operation
	 *  don't contain 2nd elememt.
	 */
	iterator erase(iterator pos) {
		Node* iter = pos.iter;
		if (iter == nullptr || pos.list != this) return pos;
		Node* nextNode = iter->next;
		
		if (iter->prev != nullptr) iter->prev->next = iter->next;
		else head = iter->next;
		if (iter->next != nullptr) iter->next->prev = iter->prev;
		else tail = iter->prev;

		delete iter;
		size--;
		return iterator(nextNode, this);
	}

	void clear()
	{
		while (head != nullptr)
		{
			Node* tmp = head;
			head = head->next;
			delete tmp;
		}
		tail = nullptr;
		size = 0;
	}

	/**
	 * the following are operations of double list
	 */
	void insert_head(const T &val) {
		Node* newNode = new Node(val);
		newNode->next = head;
		if (head != nullptr) head->prev = newNode;
		else tail = newNode;
		head = newNode;
		size++;
	}
	void insert_tail(const T &val) {
		Node* newNode = new Node(val);
		newNode->prev = tail;
		if (tail != nullptr) tail->next = newNode;
		else head = newNode;
		tail = newNode;
		size++;
	}
	void delete_head() {
		if (head == nullptr) return;
		Node* oldNode = head;
		head = head->next;
		if (head != nullptr) head->prev = nullptr;
		else tail = nullptr;
		delete oldNode;
		size--;
	}
	void delete_tail() {
		if (tail == nullptr) return;
		Node* oldNode = tail;
		tail = tail->prev;
		if (tail != nullptr) tail->next = nullptr;
		else head = nullptr;
		delete oldNode;
		size--;
	}
	/**
	 * if didn't contain anything, return true,
	 * otherwise false.
	 */
	size_t siz() const
	{
		return size;
	}

	bool empty() {
		return size == 0;
	}
};

template <class T> class deque {
private:
  struct Block
  {
    T* data;
    size_t size;

    Block(size_t n = 0) : size(n)
    {
      if (n > 0) data = new T[n];
      else data = nullptr;
    }
    ~Block()
    {
      delete[] data;
    }
  }
  double_list<Block*> blocks;
  size_t tot_size;

  size_t ideal_size()
  {
    size_t l = 1, r = tot_size;
    if (!r) return 16; // size of the first block

    while (l < r)
    {
      size_t mid = (l + r) >> 1;
      if (mid * mid < tot_size) l = mid + 1;
      else r = mid;
    }
    return l;
  }

  /*
   * locate the block idx-th iterator is in
   * also return the offset (the index of iterator in the block)
   */
  typename double_list<Block*>::iterator locate(size_t idx, size_t& offset)
  {
    if (idx >= tot_size) throw("index_out_of_bound");

    auto it = blocks.begin();
    size_t tot = 0;
    while (it != blocks.end())
    {
      Block* b = *it;
      if (idx <= tot + b->size)
      {
        offset = idx - tot;
        return it;
      }
      tot += b->size;
      it++;
    }
  }

  /*
   * split 'it' to two blocks
   */
  void split_block(typename double_list<Block*>::iterator it)
  {
    Block* oldBlock = *it;
    size_t old_size = oldBlock->size;
    size_t lsize = old_size >> 1;
    if (lsize == 0) return; // can't split
    size_t rsize = old_size - lsize;

    Block* lblock = new Block(lsize);
    Block* rblock = new Block(rsize);
    for (size_t i = 0; i < lsize; ++i) lblock->data[i] = oldBlock->data[i];
    for (size_t i = 0; i < rsize; ++i) rblock->data[i] = oldBlock->data[i + lsize];

    double_list<Block*> tmp; // temporarily store blocks after 'oldBlock'
    auto tmp_iter = it;
    tmp_iter++;
    while (tmp_iter != blocks.end())
    {
      tmp.insert_tail(*tmp_iter);
      tmp_iter = blocks.erase(tmp_iter);
    }
    *it = lblock;
    blocks.insert_tail(rblock); // remove 'old' and add two 'new's

    tmp_iter = tmp.begin(); // insert 'tmp' back to 'blocks'
    while (tmp_iter != tmp.end())
    {
      blocks.insert_tail(*tmp_iter);
      tmp_iter++;
    }

    delete oldBlock;
  }

  /*
   * merge 'lit' and 'rit' to one block
   */
  void merge_block(typename double_list<Block*>::iterator lit, typename double_list<Block*>::iterator rit)
  {
    Block* lblock = *lit;
    Block* rblock = *rit;
    size_t lsize = lblock->size;
    size_t rsize = rblock->size;
    size_t new_size = lsize + rsize;

    Block* newBlock = new Block(new_size);
    for (size_t i = 0; i < lsize; ++i) newBlock->data[i] = lblock->data[i];
    for (size_t i = 0; i < rsize; ++i) newBlock->data[i + lsize] = rblock->data[i];

    double_list<Block*> tmp;
    auto tmp_iter = rit;
    while (tmp_iter != blocks.end())
    {
      tmp.insert_tail(*tmp_iter);
      tmp_iter = blocks.erase(tmp_iter);
    }
    *lit = newBlock;

    tmp_iter = tmp.begin();
    while (tmp_iter != tmp.end())
    {
      blocks.insert_tail(*tmp_iter);
      tmp_iter++;
    }

    delete lblock;
    delete rblock;
  }

public:
  class const_iterator;
  class iterator {
  private:
    /**
     * add data members.
     * just add whatever you want.
     */
    deque* que;

  public:
    /**
     * return a new iterator which points to the n-next element.
     * if there are not enough elements, the behaviour is undefined.
     * same for operator-.
     */
    iterator operator+(const int &n) const {}
    iterator operator-(const int &n) const {}

    /**
     * return the distance between two iterators.
     * if they point to different vectors, throw
     * invaild_iterator.
     */
    int operator-(const iterator &rhs) const {}
    iterator &operator+=(const int &n) {}
    iterator &operator-=(const int &n) {}

    /**
     * iter++
     */
    iterator operator++(int) {}
    /**
     * ++iter
     */
    iterator &operator++() {}
    /**
     * iter--
     */
    iterator operator--(int) {}
    /**
     * --iter
     */
    iterator &operator--() {}

    /**
     * *it
     */
    T &operator*() const {}
    /**
     * it->field
     */
    T *operator->() const noexcept {}

    /**
     * check whether two iterators are the same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {}
    bool operator==(const const_iterator &rhs) const {}
    /**
     * some other operator for iterators.
     */
    bool operator!=(const iterator &rhs) const {}
    bool operator!=(const const_iterator &rhs) const {}
  };

  class const_iterator {
    /**
     * it should has similar member method as iterator.
     * you can copy them, but with care!
     * and it should be able to be constructed from an iterator.
     */
  };

  /**
   * constructors.
   */
  deque() {}
  deque(const deque &other) {}

  /**
   * deconstructor.
   */
  ~deque() {}

  /**
   * assignment operator.
   */
  deque &operator=(const deque &other) {}

  /**
   * access a specified element with bound checking.
   * throw index_out_of_bound if out of bound.
   */
  T &at(const size_t &pos) {}
  const T &at(const size_t &pos) const {}
  T &operator[](const size_t &pos) {}
  const T &operator[](const size_t &pos) const {}

  /**
   * access the first element.
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {}
  /**
   * access the last element.
   * throw container_is_empty when the container is empty.
   */
  const T &back() const {}

  /**
   * return an iterator to the beginning.
   */
  iterator begin() {}
  const_iterator cbegin() const {}

  /**
   * return an iterator to the end.
   */
  iterator end() {}
  const_iterator cend() const {}

  /**
   * check whether the container is empty.
   */
  bool empty() const {}

  /**
   * return the number of elements.
   */
  size_t size() const {}

  /**
   * clear all contents.
   */
  void clear() {}

  /**
   * insert value before pos.
   * return an iterator pointing to the inserted value.
   * throw if the iterator is invalid or it points to a wrong place.
   */
  iterator insert(iterator pos, const T &value) {}

  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) {}

  /**
   * add an element to the end.
   */
  void push_back(const T &value) {}

  /**
   * remove the last element.
   * throw when the container is empty.
   */
  void pop_back() {}

  /**
   * insert an element to the beginning.
   */
  void push_front(const T &value) {}

  /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() {}
};

} // namespace sjtu

#endif
