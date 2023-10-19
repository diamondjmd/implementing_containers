#pragma once

#include <iostream>
#include <stdexcept>
#include <initializer_list>

namespace cntr { //contr (container)

	template <typename T>
	class Forward_list {
			// Node of the list
			struct Node{
				Node() 
					:value{T{}}, next{nullptr} {};
				Node(const T &val, Node* next_) 
					:value{val}, next{next_}{}
				Node(T && value_, Node *next_ = nullptr) noexcept
					:value{std::move(value_)}, next{next_} {}
				
				T value{};
				Node *next;
			};

		public:
			// Constructors, destructor, assignment operators
			Forward_list(); 
			Forward_list(std::initializer_list<T> init);
			Forward_list(const Forward_list &list); //copy constructor
			Forward_list(Forward_list && list) noexcept; // move constructor
			virtual ~Forward_list() noexcept;
			
			Forward_list<T> &operator=(const Forward_list &list);
			Forward_list<T> &operator=(Forward_list &&list) noexcept;
			
			// Element access
			T &operator[](const std::size_t index);
			const T &operator[](const std::size_t index) const;
			
			// Inner classes
			class const_iterator;
			class iterator;

			// Iterators
			iterator before_begin() noexcept;
			const_iterator before_begin() const noexcept;
			const_iterator cbefore_begin() const noexcept;

			iterator begin() noexcept;
			const_iterator begin() const noexcept;
			const_iterator cbegin() const noexcept;
			iterator end() noexcept;
			const_iterator end() const noexcept;
			const_iterator cend() const noexcept;

			// Capacity
			bool empty() const;
			std::size_t size()const;
			
			// Modifiers
			iterator insert_after(const_iterator it, const T &value);
			iterator insert_after(const_iterator it, T &&value) noexcept;
			void push_front(const T &value);
			void push_front(T &&value) noexcept;
			void push_mid(const T &value);
			void push_mid(T &&value) noexcept;
			void push_back(const T &value);
			void push_back(T &&value) noexcept;
			iterator erase_after(const_iterator it);
			iterator erase_after(const std::size_t index);
			void pop_front();
			void clear();

			//Operations
			void print(const std::string & name = "") const;

		private:
			Node *head; //member
			
			// helpers
			Node *seek(const std::size_t index) noexcept;
			Node *before_end();

			template<class U>
			void push_back_items(const U &items);
	};

//-------------- Class List Implementation --------------//
	// Constructors, destructor assign operator //
	template <typename T>
	Forward_list<T>::Forward_list() :head{nullptr} {}
	
	template <typename T>
	Forward_list<T>::Forward_list(std::initializer_list<T> init) :head{nullptr} {
		push_back_items(init);
	}

	template <typename T>
	Forward_list<T>::Forward_list(const Forward_list &list) :head{nullptr}{
		*this = list;
	}

	template <typename T>
	Forward_list<T>::Forward_list(Forward_list &&list) noexcept :head{nullptr}{
		*this = std::move(list);
	}

	template <typename T>
	Forward_list<T>::~Forward_list() noexcept{
		clear();
	}

	template<typename T>
	Forward_list<T>& Forward_list<T>::operator=(const Forward_list<T> &list) {
		if (this == &list){
			return *this;
		}
		clear();
		push_back_items(list);
		return *this;
	}

	template<typename T>
	Forward_list<T>& Forward_list<T>::operator=(Forward_list<T> &&list) noexcept {
		if (this == &list){
			return *this;
		}
		clear();
		head = list.head;
		list.head = nullptr;
		return *this;
	}

	//--------------- Element access ---------------//
	template<typename T>
	T& Forward_list<T>::operator[](const std::size_t index) {
		return seek(index)->value;
	}

	template<typename T>
	const T& Forward_list<T>::operator[](const std::size_t index) const {
		return seek(index)->value;
	}

	// private member function
	template<typename T>
	typename Forward_list<T>::Node *Forward_list<T>::seek(const std::size_t index) noexcept {
		Node *temp = head;
		for (std::size_t i = 0; i < index && head->next ; ++i) {
			temp = temp->next;
		}
		return temp;
	}

	// private member function
	template<typename T>
	typename Forward_list<T>::Node *Forward_list<T>::before_end(){
		auto it = head;
		while (it && it->next){
			it = it->next;
		}
		return it;
	}

	// private member function
	template<typename T>
	template<class U>
	void Forward_list<T>::push_back_items(const U &items){
		auto it = items.begin();
		if (it != items.end()){
			auto tail = head = new Node{std::move(*it++),nullptr};
			for (; it != items.end(); ++it) {
				insert_after(const_iterator{tail}, *it);
				tail= tail->next;	
			}
		}
	}

	//-----------------  Iterators -----------------//
	template<typename T>
	typename Forward_list<T>::iterator Forward_list<T>::before_begin() noexcept { 
		return iterator{nullptr}; 
	}

	template<typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::before_begin() const noexcept { 
		return cbegin(); 
	}

	template<typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::cbefore_begin() const noexcept { 
		return const_iterator{nullptr}; 
	}

	template<typename T>
	typename Forward_list<T>::iterator Forward_list<T>::begin() noexcept { 
		return iterator{head}; 
	}

	template<typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::begin() const noexcept { 
		return cbegin(); 
	}

	template<typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::cbegin() const noexcept { 
		return const_iterator{head}; 
	}

	template<typename T>
	typename Forward_list<T>::iterator Forward_list<T>::end() noexcept { 
		return iterator{}; 
	}

	template<typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::end() const noexcept{ 
		return cend(); 
	}

	template<typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::cend() const noexcept{ 
		return const_iterator{};
	}
	
	//-----------------  Capacity ------------------//
	template<typename T>
	std::size_t Forward_list<T>::size() const {
		auto temp = head; 
		std::size_t size = 0;
		while (temp){
			++size;
			temp = temp->next;
		}
		return size; 
	}

	template<typename T>
	bool Forward_list<T>::empty() const{
		return head == nullptr; 
	}

	//-----------------  Modifiers -----------------//
	template <typename T>
	typename Forward_list<T>::iterator Forward_list<T>::insert_after(const_iterator it, const T &value){
		if (it == nullptr) {
			throw std::runtime_error("ERROR: Empty or null Iterator");
		}
		
		if (!head){
			push_front(value);
			return iterator{head};
		}

		Node *after_node =  it.current_node;
		Node *new_node = new Node(value, after_node->next);
		after_node->next = new_node;
		return iterator{new_node};
	}

	template <typename T>
	typename Forward_list<T>::iterator Forward_list<T>::insert_after(const_iterator it, T &&value) noexcept {
		if (!head){
			push_front(std::move(value));
			return iterator{head};
		}

		Node *after_node =  it.current_node;
		Node *new_node = new Node(std::move(value), after_node->next);
		after_node->next = new_node;
		return iterator{new_node};
	}

	template <typename T>
	void Forward_list<T>::push_front(const T &value) {
		auto new_node = new Node{value, head};
		head = new_node;
	}

	template <typename T>
	void Forward_list<T>::push_front(T &&value) noexcept {
		auto new_node = new Node{std::move(value), head};
		head = new_node;
	}

	template <typename T>
	void Forward_list<T>::push_mid(const T &value) {
		if(!empty()){
			insert_after(const_iterator(seek(size() / 2 - 1)), value); 
		}else {
			push_front(value);
		}
	}

	template <typename T>
	void Forward_list<T>::push_mid(T &&value) noexcept {
		if(!empty()){
			insert_after (const_iterator(seek(size() / 2 - 1)), std::move(value)); 
		}else {
			push_front(std::move(value));
		}
	}

	template <typename T>
	void Forward_list<T>::push_back(const T &value) {
		insert_after(const_iterator(before_end()), value); 
	}

	template <typename T>
	void Forward_list<T>::push_back(T &&value) noexcept {
		insert_after(const_iterator(before_end()), std::move(value)); 
	}

	template <typename T>
	typename Forward_list<T>::iterator Forward_list<T>::erase_after(const_iterator it){
		Node *current = it.current_node;
		Node *to_return = nullptr;
		
		if (current && current->next){
			 auto to_delete = current->next;
			 to_return = current->next = current->next->next;
			 delete to_delete;
		} else if(!current && head){
			auto to_delete = head;
			to_return = head = head->next;
			delete to_delete; 
		}
		return iterator{to_return};
	}

	template <typename T>
	typename Forward_list<T>::iterator Forward_list<T>::erase_after(const std::size_t index) {
		return erase_after(const_iterator(seek(index)));
	}

	template <typename T>
	void Forward_list<T>::pop_front() {
		if (empty()) { 
			throw std::runtime_error("ERROR: Empty container");
		}
		erase_after(before_begin());
	}

	template<typename T>
	void Forward_list<T>::clear() {
		if (!head) {
			return;
		}
		
		Node *prev = nullptr;
		while (head->next) {
			prev = head;
			head = head->next;
			delete prev;
		}
		delete head;

		head = nullptr;
	}

	//-----------------  Operations -----------------//
	template<typename T>
	void Forward_list<T>::print(const std::string & name) const{

		std::cout << "\n<===== Forward List: " << name << " ======>";
		std::size_t index = 0;
		for (const auto &it : *this) {
			std::cout << "\n [" << index++ << "]=> " << it ;
			//index++;
		}
		std::cout << "\n<=== End " << name << " ====>\n";
	}

	//---------------- Non-member functions ----------------//
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Forward_list<T> & list) {
		for (const auto &it : list) {
			os << it << "->";
		}
		os << "NULL";

		return os;
	}

	//-------------- Inner class const_iterator --------//
	template <typename T>	
	class Forward_list<T>::const_iterator {
	public:
		const_iterator();
		
		const T & operator*() const;
		const_iterator & operator++(); // Prefix
		const_iterator operator++(int);// Postfix
		bool operator==(const const_iterator & other) const;
		bool operator!=(const const_iterator & other) const;

	protected:
		Node *current_node{}; // member

		const_iterator(Node *new_ptr); // constructor
		T &get() const; // get the value at the iterator current position
		friend class Forward_list<T>;
	};
			
	//-------------- Inner class iterator --------//
	template <typename T>
	class Forward_list<T>::iterator final: public const_iterator {
	public:
		iterator();

		T &operator*();
		const T &operator*() const;

		iterator &operator++();
		iterator operator++(int);

	private:
		iterator(Node *new_ptr); // constructor
		friend class Forward_list<T>;			
	};

	//-------------- class const_iterator implementation--------//
	template <typename T>
	Forward_list<T>::const_iterator::const_iterator() :current_node{nullptr} {}

	//protected constructor
	template <typename T>
	Forward_list<T>::const_iterator::const_iterator (Node *new_ptr) :current_node{new_ptr} {}

	template <typename T>
	const T &Forward_list<T>::const_iterator::operator*() const{
		return get();
	}

	template <typename T>
	T &Forward_list<T>::const_iterator::get() const{
		return current_node->value;
	}

	template <typename T>
	typename Forward_list<T>::const_iterator &Forward_list<T>::const_iterator::operator++(){ // Prefix
		current_node = current_node -> next;
		return *this;
	}

	template <typename T>
	typename Forward_list<T>::const_iterator Forward_list<T>::const_iterator::operator++(int){ // Postfix
		const_iterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	bool Forward_list<T>::const_iterator::operator==(const const_iterator &other) const {
		return current_node == other.current_node;
	}

	template <typename T>
	bool Forward_list<T>::const_iterator::operator!=(const const_iterator &other) const {
		return !(*this == other);
	}

	//-------------- Class iterator implementation --------//
	template <typename T>
	Forward_list<T>::iterator::iterator() :const_iterator{} {}

	template <typename T>
	Forward_list<T>::iterator::iterator(Node *new_ptr)  :const_iterator {new_ptr} {}

	template <typename T>
	const T &Forward_list<T>::iterator::operator*() const {
		return const_iterator::operator*();
	}

	template <typename T>
	T &Forward_list<T>::iterator::operator*() {
		return const_iterator::get();
	}

	template <typename T>
	typename Forward_list<T>::iterator &Forward_list<T>::iterator::operator++(){
		this->current_node = this->current_node->next;
		return *this;
	}

	template <typename T>
	typename Forward_list<T>::iterator Forward_list<T>::iterator::operator++(int){
		iterator temp = *this;
		++(*this);
		return temp;
	}
} // namespace  cntr (container)