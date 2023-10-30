#pragma once

#include <stdexcept>
#include <initializer_list>
#include <sstream>

namespace container {

	template <typename T>
	class List {
			// Node of the list
			struct Node{
				Node()
					:value{T{}}, next{nullptr}, prev{nullptr} {};
				Node(const T &val, Node* next_ , Node* prev_)
					:value{val}, next{next_}, prev{prev_} {}

				T value{};
				Node *next;
				Node *prev;
			};

		public:

			// Constructors, destructor, assignment operators
			List();
			List(const List &list); //copy constructor
			List(List && list) noexcept; // move constructor
			List(const std::initializer_list<T> &elements); //initializer list constructor
			virtual ~List();

			// List<T> &operator=(const List &list);
			List<T> &operator=(const List &list); // implements copy swap idiom
			List<T> &operator=(List &&list) noexcept;

			// Element access
			T &operator[](const std::size_t index);
			const T &operator[](const std::size_t index) const;

			// Inner classes
			class const_iterator;
			class iterator;

			// Iterators
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
			void clear();
			iterator insert(const_iterator it, const T &value);
			iterator insert(const_iterator it, T &&value) noexcept;
			void push_front(const T &value);
			void push_front(T &&value) noexcept;
			void push_mid(const T &value);
			void push_mid(T &&value) noexcept;
			void push_back(const T &value);
			void push_back(T &&value) noexcept;
			iterator erase(const_iterator it);
			iterator erase(const std::size_t index);
			void pop_front();
			void pop_back();
			void swap(List<T> &list) noexcept;


			//Operations
			void reverse();
			std::string toString(const std::string &name = "") const;

	private:
			Node *head;
			Node *tail;
			std::size_t m_size{};

			// helper
			Node *seek(const std::size_t index);
	};

//-------------- Class List Implementation --------------//
	// Constructors, destructor assign operator //
	template <typename T>
	List<T>::List() :head{new Node{}}, tail{head}, m_size{} {}

	template <typename T>
	List<T>::List(const List<T> &list) :List{} {
		for (const auto &it : list) {
			push_back(it);
		}
	}

	template <typename T>
	List<T>::List(List &&list) noexcept :List{} {
		swap(list);
	}

	template <typename T>
	List<T>::List(const std::initializer_list<T> &elements) :List{} {
		for (auto &element: elements)	{
			push_back(element);
		}
	}

	template <typename T>
	List<T>::~List(){
		clear();
	}

	template<typename T>
	List<T>& List<T>::operator=(const List<T> &list){
		if (this != &list){
			auto temp{list};
			swap(temp);
		}
		return *this;
	}

	template<typename T>
	List<T>& List<T>::operator=(List<T> &&list) noexcept {
		if (this != &list){
			swap(list);
		}
		return *this;
	}

	//--------------- Element access ---------------//
	template<typename T>
	T& List<T>::operator[](const std::size_t index) {
		return seek(index)->value;
	}

	template<typename T>
	const T& List<T>::operator[](const std::size_t index) const {
		return seek(index)->value;
	}

	// private member function
	template<typename T>
	typename List<T>::Node *List<T>::seek(const std::size_t index) {
		if (index >= m_size) {
			throw std::out_of_range("ERROR: Index out of bounds in List");
		}
		Node *temp = nullptr;
		if (index < m_size / 2) {
			// normal way
			temp = head;
			for (size_t i = 0; i < index; ++i) {
				temp = temp->next;
			}
		}else {
			// reverse way
			temp = tail;
			for (size_t i = m_size; i > index; --i) {
				temp = temp->prev;
			}
		}
		return temp;
	}

	//-----------------  Iterators -----------------//
	template<typename T>
	typename List<T>::iterator List<T>::begin() noexcept {
		return iterator{head};
	}

	template<typename T>
	typename List<T>::const_iterator List<T>::begin() const noexcept {
		return cbegin();
	}

	template<typename T>
	typename List<T>::const_iterator List<T>::cbegin() const noexcept {
		return const_iterator{head};
	}

	template<typename T>
	typename List<T>::iterator List<T>::end() noexcept {
		return iterator{tail};
	}

	template<typename T>
	typename List<T>::const_iterator List<T>::end() const noexcept{
		return cend();
	}

	template<typename T>
	typename List<T>::const_iterator List<T>::cend() const noexcept{
		return const_iterator{tail};
	}

	//-----------------  Capacity ------------------//
	template<typename T>
	std::size_t List<T>::size() const {
		return m_size;
	}

	template<typename T>
	bool List<T>::empty() const{
		return head == tail;
	}

	//-----------------  Modifiers -----------------//
	template<typename T>
	void List<T>::clear() {
		if (!head) return;

		while (head->next) {
			head = head->next;
			delete head->prev;
		}
		delete head;
		head = tail = nullptr;
		m_size = 0;
	}

	template <typename T>
	typename List<T>::iterator List<T>::insert(const_iterator it, const T &value){
		if (it == nullptr && m_size) {
			throw std::runtime_error("ERROR: Empty or null Iterator");
		}

		Node *current = it.current_node;
		Node *new_node = new Node(value, current, current->prev);
		if (new_node->prev) {
				new_node->prev->next = new_node;
		}
		current->prev = new_node;
		m_size++;

		return (new_node);
	}

	template <typename T>
	typename List<T>::iterator List<T>::insert(const_iterator it, T &&value) noexcept {
		Node *current = it.current_node;
		Node *new_node = new Node(std::move(value), current, current->prev);
		if (new_node->prev) {
				new_node->prev->next = new_node;
		}
		current->prev = new_node;
		m_size++;
		return iterator(new_node);
	}

	template <typename T>
	void List<T>::push_front(const T &value) {
		head = insert (begin(), value).current_node;
	}

	template <typename T>
	void List<T>::push_front(T &&value) noexcept {
		head = insert (begin(), std::move(value)).current_node;
	}

	template <typename T>
	void List<T>::push_mid(const T &value) {
		if(!empty()){
			insert (const_iterator(seek(m_size/2)), value);
		}else {
			push_front(value);
		}
	}

	template <typename T>
	void List<T>::push_mid(T &&value) noexcept {
		if(!empty()){
			insert (const_iterator(seek(m_size/2)), std::move(value));
		}else {
			push_front(std::move(value));
		}
	}

	template <typename T>
	void List<T>::push_back(const T &value) {
		if(!empty()){
			insert (end(), value);
		}else {
			push_front(value);
		}
	}

	template <typename T>
	void List<T>::push_back(T &&value) noexcept {
		if(!empty()){
			insert (end(), std::move(value));
		}else {
			push_front(std::move(value));
		}
	}

	template <typename T>
	typename List<T>::iterator List<T>::erase(const_iterator it) {
		Node *current = it.current_node;
		iterator to_return{current};
		if (!head || !current){
			return to_return;
		}
		if (current->prev){
			current->prev->next = current->next;
		}else{
			head = current->next;
		}

		if (current->next){
			current->next->prev = current->prev;
		}else{
			tail = current->prev;
		}

		delete current;
		--m_size;
		return to_return;
	}

	template <typename T>
	typename List<T>::iterator List<T>::erase(const std::size_t index) {
		return erase(const_iterator(seek(index)));
	}

	template <typename T>
	void List<T>::pop_front() {
		if (empty()) {
			throw std::runtime_error("ERROR: Empty container");
		}
		erase(begin());
	}

	template <typename T>
	void List<T>::pop_back() {
		if (empty()) {
			throw std::runtime_error("ERROR: Empty container");
		}
		erase(end());
	}

	template <typename T>
	void List<T>::swap(List<T> &list) noexcept {
		std::swap(head, list.head);
		std::swap(tail, list.tail);
		std::swap(m_size, list.m_size);
	}

	//-----------------  Operations -----------------//
	template<typename T>
	void List<T>::reverse(){
		List<T> temp_list;

		for (const auto &it : *this){
			temp_list.push_front(it);
		}

		*this = temp_list;
	}

	template<typename T>
	std::string List<T>::toString(const std::string & name) const {
		std::stringstream stream;
		stream << "\n<===== List: " << name << " ======>\n >>Size:" << m_size;
		std::size_t index = 0;
		for (const auto &it : *this) {
			stream << "\n [" << index << "]=> " << it ;
			index++;
		}
		stream << "\n<=== End " << name << " ====>\n";
		return stream.str();
	}

	//---------------- Non-member functions ----------------//
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const List<T> & list) {
		for (const auto &it : list) {
			os << it << "->";
		}
		os << "NULL";

		return os;
	}

	//-------------- Inner class const_iterator --------//
	template <typename T>
	class List<T>::const_iterator {
	public:
		const_iterator();

		const T & operator*() const;
		const_iterator & operator++(); // Prefix
		const_iterator operator++(int);// Postfix
		const_iterator & operator--(); // Prefix
		const_iterator operator--(int);// Postfix
		bool operator==(const const_iterator & other) const;
		bool operator!=(const const_iterator & other) const;

	protected:
		Node *current_node{}; // member

		const_iterator(Node *new_ptr); // constructor
		T &get() const; // get the value at the iterator current position
		friend class List<T>;
	};

	//-------------- Inner class iterator --------//
	template <typename T>
	class List<T>::iterator final: public const_iterator {
	public:
		iterator();

		T &operator*();
		const T &operator*() const;

		iterator &operator++();
		iterator operator++(int);
		iterator &operator--();
		iterator operator--(int);

	private:
		iterator(Node *new_ptr); // constructor
		friend class List<T>;
	};

	//-------------- class const_iterator implementation--------//
	template <typename T>
	List<T>::const_iterator::const_iterator() :current_node{nullptr} {}

	//protected constructor
	template <typename T>
	List<T>::const_iterator::const_iterator (Node *new_ptr) :current_node{new_ptr} {}

	template <typename T>
	const T &List<T>::const_iterator::operator*() const{
		return get();
	}

	template <typename T>
	T &List<T>::const_iterator::get() const{
		return current_node->value;
	}

	template <typename T>
	typename List<T>::const_iterator &List<T>::const_iterator::operator++(){ // Prefix
		current_node = current_node -> next;
		return *this;
	}

	template <typename T>
	typename List<T>::const_iterator List<T>::const_iterator::operator++(int){ // Postfix
		const_iterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	typename List<T>::const_iterator &List<T>::const_iterator::operator--(){ // Prefix
		current_node = current_node->prev;
		return *this;
	}

	template <typename T>
	typename List<T>::const_iterator List<T>::const_iterator::operator--(int){ // Postfix
		const_iterator temp = *this;
		--(*this);
		return temp;
	}

	template <typename T>
	bool List<T>::const_iterator::operator==(const const_iterator &other) const {
		return current_node == other.current_node;
	}

	template <typename T>
	bool List<T>::const_iterator::operator!=(const const_iterator &other) const {
		return !(*this == other);
	}

	//-------------- Class iterator implementation --------//
	template <typename T>
	List<T>::iterator::iterator() :const_iterator{} {}

	template <typename T>
	List<T>::iterator::iterator(Node *new_ptr)  :const_iterator {new_ptr} {}

	template <typename T>
	const T &List<T>::iterator::operator*() const {
		return const_iterator::operator*();
	}

	template <typename T>
	T &List<T>::iterator::operator*() {
		return const_iterator::get();
	}

	template <typename T>
	typename List<T>::iterator &List<T>::iterator::operator++(){
		this->current_node = this->current_node->next;
		return *this;
	}

	template <typename T>
	typename List<T>::iterator List<T>::iterator::operator++(int){
		iterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	typename List<T>::iterator &List<T>::iterator::operator--(){
		this->current_node = this->current_node->prev;
		return *this;
	}

	template <typename T>
	typename List<T>::iterator List<T>::iterator::operator--(int){
		iterator temp = *this;
		--(*this);
		return temp;
	}
} // namespace  container