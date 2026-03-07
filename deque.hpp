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

  class const_iterator;
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

  class const_iterator {
	private:
		friend class double_list;
		const Node* iter;
		const double_list* list;
	public:
		const_iterator(const Node* it = nullptr, const double_list* l = nullptr) : iter(it), list(l) {}
		const_iterator(const const_iterator &t) : iter(t.iter), list(t.list) {}
    const_iterator(const iterator& t) : iter(t.iter), list(t.list) {}
		~const_iterator() = default;
		
		/**
		 * iter++
		 */
		const_iterator operator++(int) {
			if (iter == nullptr) throw("invalid");
			const_iterator tmp = *this;
			iter = iter->next;
			return tmp;
		}
		/**
		 * ++iter
		 */
		const_iterator &operator++() {
			if (iter == nullptr) throw("invalid");
			iter = iter->next;
			return *this;
		}
		/**
		 * iter--
		 */
		const_iterator operator--(int) {
			if (iter == nullptr)
			{
				if (list == nullptr || list->tail == nullptr) throw("invalid");
				const_iterator tmp = *this;
				iter = list->tail;
				return tmp;
			}
			else
			{
				if (iter->prev == nullptr) throw("invalid");
				const_iterator tmp = *this;
				iter = iter->prev;
				return tmp;
			}
		}
		/**
		 * --iter
		 */
		const_iterator &operator--() {
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
		const T &operator*() const {
			if (iter == nullptr) throw("invalid");
			return iter->data;
		}
		
		/**
		 * other operation
		 */
		const T *operator->() const noexcept {
			return &(iter->data);
		}
		bool operator==(const const_iterator &rhs) const {
			return iter == rhs.iter && list == rhs.list;
		}
		bool operator!=(const const_iterator &rhs) const {
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

  const_iterator cbegin() const {
		return const_iterator(head, this);
	}
	const_iterator cend() const {
		return const_iterator(nullptr, this);
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
      if (n > 0) data = static_cast<T*>(::operator new(sizeof(T) * n));
      else data = nullptr;
    }
    ~Block()
    {
      if (data)
      {
        for (size_t i = 0; i < size; ++i)
          data[i].~T();
        ::operator delete(data);
      }
    }

    void construct_at(size_t pos, const T& val) // construct T() (not default)
    {
      new (&data[pos]) T(val);
    }
  };
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
    if (idx == tot_size) return blocks.end();
    if (idx > tot_size) throw("index_out_of_bound");

    auto it = blocks.begin();
    size_t tot = 0;
    while (it != blocks.end())
    {
      Block* b = *it;
      if (idx < tot + b->size) // idx \in [0, b->size())
      {
        offset = idx - tot;
        return it;
      }
      tot += b->size;
      it++;
    }
    throw("index_out_of_bound");
  }

  typename double_list<Block*>::const_iterator locate(size_t idx, size_t& offset) const
  {
    if (idx == tot_size) return blocks.cend();
    if (idx > tot_size) throw("index_out_of_bound");

    auto it = blocks.cbegin();
    size_t tot = 0;
    while (it != blocks.cend())
    {
      Block* b = *it;
      if (idx < tot + b->size) // idx \in [0, b->size())
      {
        offset = idx - tot;
        return it;
      }
      tot += b->size;
      it++;
    }
    throw("index_out_of_bound");
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
    for (size_t i = 0; i < lsize; ++i) lblock->construct_at(i, oldBlock->data[i]);
    for (size_t i = 0; i < rsize; ++i) rblock->construct_at(i, oldBlock->data[i + lsize]);

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
    for (size_t i = 0; i < lsize; ++i) newBlock->construct_at(i, lblock->data[i]);
    for (size_t i = 0; i < rsize; ++i) newBlock->construct_at(i + lsize, rblock->data[i]);

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
    friend class const_iterator;
    friend class deque;
    typename double_list<Block*>::iterator iter;
    deque* que; // the deque it belongs to
    size_t offset; // the offset inside the block

  public:
    /**
     * return a new iterator which points to the n-next element.
     * if there are not enough elements, the behaviour is undefined.
     * same for operator-.
     */
    iterator(typename double_list<Block*>::iterator it = typename double_list<Block*>::iterator(), deque* q = nullptr,  size_t off = 0)
      : iter(it), que(q), offset(off) {}
    iterator(const iterator& other) : iter(other.iter), que(other.que), offset(other.offset) {}
    ~iterator() = default;

    /*
     * a function used to calculate the global index of an iterator (element)
     */
    size_t cur_idx() const
    {
      if (que == nullptr) throw("invalid_iterator");
      size_t idx = offset;
      auto it = que->blocks.begin();
      
      while (it != iter)
      {
        idx += (*it)->size;
        it++;
      }

      return idx;
    }

    iterator operator+(const int &n) const {
      size_t idx = cur_idx() + n;
      size_t off = 0;
      auto newBlock = que->locate(idx, off);
      return iterator(newBlock, que, off);
    }
    iterator operator-(const int &n) const {
      return *this + (-n);
    }

    /**
     * return the distance between two iterators.
     * if they point to different vectors, throw
     * invaild_iterator.
     */
    int operator-(const iterator &rhs) const {
      if (que != rhs.que) throw("invalid_iterator");
      return static_cast<int>(cur_idx() - rhs.cur_idx());
    }
    iterator &operator+=(const int &n) {
      *this = *this + n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      *this = *this - n;
      return *this;
    }

    /**
     * iter++
     */
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    /**
     * ++iter
     */
    iterator &operator++() {
      if (offset + 1 == (*iter)->size)
      {
        iter++;
        offset = 0;
      }
      else offset++;
      return *this;
    }
    /**
     * iter--
     */
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }
    /**
     * --iter
     */
    iterator &operator--() {
      if (offset == 0)
      {
        iter--;
        offset = (*iter)->size - 1;
      }
      else offset--;
      return *this;
    }

    /**
     * *it
     */
    T &operator*() const {
      return (*iter)->data[offset]; // if invalid, (*iter) will throw in double_list
    }
    /**
     * it->field
     */
    T *operator->() const noexcept {
      return &(*iter)->data[offset];
    }

    /**
     * check whether two iterators are the same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      return iter == rhs.iter && que == rhs.que && offset == rhs.offset;
    }
    bool operator==(const const_iterator &rhs) const {
      return iter == rhs.iter && que == rhs.que && offset == rhs.offset;
    }
    /**
     * some other operator for iterators.
     */
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }
  };

  class const_iterator {
    /**
     * it should has similar member method as iterator.
     * you can copy them, but with care!
     * and it should be able to be constructed from an iterator.
     */
    private:
    friend class iterator;
    friend class deque;
    typename double_list<Block*>::const_iterator iter;
    const deque* que;
    size_t offset;

  public:
    const_iterator(typename double_list<Block*>::const_iterator it = typename double_list<Block*>::const_iterator(), const deque* q = nullptr,  size_t off = 0)
      : iter(it), que(q), offset(off) {}
    const_iterator(const iterator& other) : iter(other.iter), que(other.que), offset(other.offset) {}
    const_iterator(const const_iterator& other) : iter(other.iter), que(other.que), offset(other.offset) {}
    ~const_iterator() = default;

    size_t cur_idx() const
    {
      if (que == nullptr) throw("invalid_iterator");
      size_t idx = offset;
      auto it = que->blocks.cbegin();
      
      while (it != iter)
      {
        idx += (*it)->size;
        it++;
      }

      return idx;
    }

    const_iterator operator+(const int &n) const {
      size_t idx = cur_idx() + n;
      size_t off = 0;
      auto newBlock = que->locate(idx, off);
      return const_iterator(newBlock, que, off);
    }
    const_iterator operator-(const int &n) const {
      return *this + (-n);
    }

    int operator-(const const_iterator &rhs) const {
      if (que != rhs.que) throw("invalid_iterator");
      return static_cast<int>(cur_idx() - rhs.cur_idx());
    }
    const_iterator &operator+=(const int &n) {
      *this = *this + n;
      return *this;
    }
    const_iterator &operator-=(const int &n) {
      *this = *this - n;
      return *this;
    }

    /**
     * iter++
     */
    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    /**
     * ++iter
     */
    const_iterator &operator++() {
      if (offset + 1 == (*iter)->size)
      {
        iter++;
        offset = 0;
      }
      else offset++;
      return *this;
    }
    /**
     * iter--
     */
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    /**
     * --iter
     */
    const_iterator &operator--() {
      if (offset == 0)
      {
        iter--;
        offset = (*iter)->size - 1;
      }
      else offset--;
      return *this;
    }

    /**
     * *it
     */
    const T &operator*() const {
      return (*iter)->data[offset]; // if invalid, (*iter) will throw in double_list
    }
    /**
     * it->field
     */
    const T *operator->() const noexcept {
      return &(*iter)->data[offset];
    }

    /**
     * check whether two iterators are the same (pointing to the same
     * memory).
     */
    bool operator==(const const_iterator &rhs) const {
      return iter == rhs.iter && que == rhs.que && offset == rhs.offset;
    }
    bool operator==(const iterator &rhs) const {
      return iter == rhs.iter && que == rhs.que && offset == rhs.offset;
    }
    /**
     * some other operator for iterators.
     */
    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }
  };

  /**
   * constructors.
   */
  deque() : tot_size(0) {}
  deque(const deque &other) : tot_size(other.tot_size) {
    for (auto it = other.blocks.cbegin(); it != other.blocks.cend(); ++it)
    {
      Block* b = *it;
      size_t siz = b->size;
      Block* newBlock = new Block(siz);
      for (size_t i = 0; i < siz; ++i) newBlock->construct_at(i, b->data[i]);
      blocks.insert_tail(newBlock);
    }
  }

  /**
   * deconstructor.
   */
  ~deque() {
    clear();
  }

  /**
   * assignment operator.
   */
  deque &operator=(const deque &other) {
    if (this != &other)
    {
      clear();
      for (auto it = other.blocks.cbegin(); it != other.blocks.cend(); ++it)
      {
        Block* b = *it;
        size_t siz = b->size;
        Block* newBlock = new Block(siz);
        for (size_t i = 0; i < siz; ++i) newBlock->construct_at(i, b->data[i]);
        blocks.insert_tail(newBlock);
      }
      tot_size = other.tot_size;
    }
    return *this;
  }

  /**
   * access a specified element with bound checking.
   * throw index_out_of_bound if out of bound.
   */
  T &at(const size_t &pos) {
    if (pos < 0 || pos >= tot_size) throw("index_out_of_bound");

    size_t off = 0;
    auto it = locate(pos, off);
    return (*it)->data[off];
  }
  const T &at(const size_t &pos) const {
    if (pos < 0 || pos >= tot_size) throw("index_out_of_bound");

    size_t off = 0;
    auto it = locate(pos, off);
    return (*it)->data[off];
  }
  T &operator[](const size_t &pos) {
    return at(pos);
  }
  const T &operator[](const size_t &pos) const {
    return at(pos);
  }

  /**
   * access the first element.
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {
    if (empty()) throw("container_is_empty");
    return *cbegin();
  }
  /**
   * access the last element.
   * throw container_is_empty when the container is empty.
   */
  const T &back() const {
    if (empty()) throw("container_is_empty");
    auto it = cend();
    --it;
    return *it;
  }

  /**
   * return an iterator to the beginning.
   */
  iterator begin() {
    if (empty()) return end();
    return iterator(blocks.begin(), this, 0);
  }
  const_iterator cbegin() const {
    if (empty()) return cend();
    return const_iterator(blocks.cbegin(), this, 0);
  }

  /**
   * return an iterator to the end.
   */
  iterator end() {
    return iterator(blocks.end(), this, 0);
  }
  const_iterator cend() const {
    return const_iterator(blocks.cend(), this, 0);
  }

  /**
   * check whether the container is empty.
   */
  bool empty() const { return tot_size == 0; }

  /**
   * return the number of elements.
   */
  size_t size() const { return tot_size; }

  /**
   * clear all contents.
   */
  void clear() {
    auto it = blocks.begin();
    while (it != blocks.end())
    {
      delete (*it);
      it = blocks.erase(it);
    }
    tot_size = 0;
  }

  /**
   * insert value before pos.
   * return an iterator pointing to the inserted value.
   * throw if the iterator is invalid or it points to a wrong place.
   */
  iterator insert(iterator pos, const T &value) {
    if (pos.que != this) throw("invalid_iterator");
    if (pos == end())
    { 
      push_back(value);
      auto it = end();
      --it;
      return it;
    }
    size_t pos_idx = pos.cur_idx();
    if (pos_idx < 0 || pos_idx > tot_size) throw("index_out_of_bound");

    size_t off = pos.offset;
    auto iter = pos.iter;
    Block* curBlock = *iter;
    size_t goal = ideal_size();

    if (curBlock->size + 1 > (goal << 1)) // should be split
    {
      split_block(iter);
      size_t idx = pos.cur_idx(); // calculate the numbers again
      off = 0;
      iter = locate(idx, off);
      curBlock = *iter;
    }

    // delete oldData and calculate newData
    Block* newBlock = new Block(curBlock->size + 1);
    for (size_t i = 0; i < off; ++i) newBlock->construct_at(i, curBlock->data[i]);
    newBlock->construct_at(off, value);
    for (size_t i = off; i < curBlock->size; ++i) newBlock->construct_at(i + 1, curBlock->data[i]);
    delete curBlock;
    *iter = newBlock;

    tot_size++;
    return iterator(iter, this, off);
  }

  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) {
    if (pos.que != this) throw("invalid_iterator");
    if (empty()) throw("container_is_empty");
    size_t pos_idx = pos.cur_idx();
    if (pos_idx < 0 || pos_idx >= tot_size) throw("index_out_of_bound");

    size_t off = pos.offset;
    auto iter = pos.iter;
    Block* curBlock = *iter;

    if (curBlock->size == 1) // if size = 1, delete the whole block
    {
      auto next_it = iter;
      ++next_it;
      delete curBlock;
      blocks.erase(iter);
      tot_size--;
      return iterator(next_it, this, 0);
    }

    Block* newBlock = new Block(curBlock->size - 1);
    for (size_t i = 0; i < off; ++i) newBlock->construct_at(i, curBlock->data[i]);
    for (size_t i = off + 1; i < curBlock->size; ++i) newBlock->construct_at(i - 1, curBlock->data[i]);
    delete curBlock;
    *iter = newBlock;
    tot_size--;
    curBlock = *iter;

    size_t goal = ideal_size();
    if (curBlock->size < (goal >> 1) && blocks.siz() > 1) // should be merged
    {
      auto prev = iter;
      auto next = iter;
      next++;
      if (prev != blocks.begin())
      {
        prev--;
        merge_block(prev, iter);
        iter = prev;
        iter++;
      }
      else if (next != blocks.end())
      {
        merge_block(iter, next);
      }
    }

    // check if the deleted element is the last one
    if (off < curBlock->size) return iterator(iter, this, off);
    else
    {
      iter++;
      return iterator(iter, this, 0);
    }
  }

  /**
   * add an element to the end.
   */
  void push_back(const T &value) {
    size_t goal = ideal_size();
    auto last = blocks.end();
    if (blocks.siz() > 0) last--;

    if (blocks.siz() == 0 || (*last)->size >= (goal << 1)) // cases to add new blocks
    {
      Block* newBlock = new Block(1);
      newBlock->construct_at(0, value);
      blocks.insert_tail(newBlock);
    }
    else
    {
      Block* blk = *last;
      Block* newBlock = new Block(blk->size + 1);
      for (size_t i = 0; i < blk->size; ++i) newBlock->construct_at(i, blk->data[i]);
      newBlock->construct_at(blk->size, value);
      delete blk;
      *last = newBlock;
    }
    tot_size++;
  }

  /**
   * remove the last element.
   * throw when the container is empty.
   */
  void pop_back() {
    if (empty()) throw("container_is_empty");

    auto last = blocks.end();
    last--;
    Block* blk = *last;
    if (blk->size == 1) // only one element, delete the block
    {
      delete blk;
      blocks.delete_tail();
    }
    else
    {
      Block* newBlock = new Block(blk->size - 1);
      for (size_t i = 0; i < blk->size - 1; ++i) newBlock->construct_at(i, blk->data[i]);
      delete blk;
      *last = newBlock;
    }
    tot_size--;
  }

  /**
   * insert an element to the beginning.
   */
  void push_front(const T &value) {
    size_t goal = ideal_size();
    auto first = blocks.begin();

    if (blocks.siz() == 0 || (*first)->size >= (goal << 1)) // cases to add new blocks
    {
      Block* newBlock = new Block(1);
      newBlock->construct_at(0, value);
      blocks.insert_head(newBlock);
    }
    else
    {
      Block* blk = *first;
      Block* newBlock = new Block(blk->size + 1);
      newBlock->construct_at(0, value);
      for (size_t i = 0; i < blk->size; ++i) newBlock->construct_at(i + 1, blk->data[i]);
      delete blk;
      *first = newBlock;
    }
    tot_size++;
  }

  /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() {
    if (empty()) throw("container_is_empty");

    auto first = blocks.begin();
    Block* blk = *first;
    if (blk->size == 1) // only one element, delete the block
    {
      delete blk;
      blocks.delete_head();
    }
    else
    {
      Block* newBlock = new Block(blk->size - 1);
      for (size_t i = 1; i < blk->size; ++i) newBlock->construct_at(i - 1, blk->data[i]);
      delete blk;
      *first = newBlock;
    }
    tot_size--;
  }
};

} // namespace sjtu

#endif
