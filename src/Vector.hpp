#pragma once

#include <stdexcept>
#include <initializer_list>
#include <sstream>

namespace container {

    template<typename T>
    class Vector {
    public:
        // Constructors and destructor
        Vector();
        explicit Vector(std::size_t count);
        Vector(const Vector& other); // copy constructor
        Vector(Vector &&other) noexcept; //move constructor
        Vector(std::initializer_list<T> elements);
        virtual ~Vector();
        Vector<T> &operator=(const Vector &other); // applies copy and swap idiom
        Vector<T> &operator=(Vector &&other) noexcept; // applies copy and swap idiom

        // Element access
        T &at(std::size_t index);
        const T &at(std::size_t index) const;
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
        void reserve(std::size_t new_cap);
        std::size_t size() const;


        // Modifiers
        iterator insert(const_iterator pos, const T &value);
        iterator insert(const_iterator pos, T &&value) noexcept;
        iterator insert(std::size_t pos, const T &value);
        iterator insert(std::size_t pos, T &&value);
        void clear() noexcept;
        iterator erase(const_iterator pos);
        iterator erase(const std::size_t pos);
        void push_back(const T &value);
        void swap(Vector &vector) noexcept;


        //Operations
        std::string toString(const std::string &name = "") const;

    private:
        std::size_t m_size;
        std::size_t m_capacity;
        T *m_data;

        void move_data(T *from, T *to, std::size_t count);
        T *check_to_insert(T *pos);
    };

//-------------- Class Vector Implementation ------------//
    //------ Constructors, destructor ----------//
    template<typename T>
    Vector<T>::Vector() :m_size{}, m_capacity{}, m_data{nullptr} {};

    template<typename T>
    Vector<T>::Vector(const Vector &other) :m_size{other.m_size}, m_capacity{other.m_capacity} {
        m_data = new T[m_size];
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }

    template<typename T>
    Vector<T>::Vector(Vector &&other) noexcept :Vector{} {
        swap(other);
    }

    template<typename T>
    Vector<T>::Vector(std::size_t count)
        :m_size{count}, m_capacity{count}, m_data{new T[count]} {}

    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> elements) :Vector(elements.size()) {
        std::size_t i = 0;
        for (auto& element :elements){
            m_data[i++] = element;
        }
    }

    template<typename T>
    Vector<T>::~Vector(){
        clear();
    }

    // applying copy-and-swap idiom
    template<typename T>
    Vector<T> &Vector<T>::operator=(const Vector &other) {
        if (this != &other){
            Vector temp{other};
            swap(temp);
        }
        return *this;
    }

    // applying copy-and-swap idiom
    template<typename T>
    Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {
        if (this != &other){
            swap(other);
        }
        return *this;
    }
    //--------------- Element access ---------------//
    template<typename T>
    T &Vector<T>::at(std::size_t index){
        if (index >= m_size){
		    throw std::out_of_range("ERROR: Index out of bounds in Vector");
        }
        return m_data[index];
    }

    template<typename T>
    const T &Vector<T>::at(std::size_t index) const {
        return at(index);
    }

    template<typename T>
    T &Vector<T>::operator[](const std::size_t index){
        return m_data[index];
    }

    template<typename T>
    const T &Vector<T>::operator[](const std::size_t index) const {
        return m_data[index];
    }

    //-----------------  Iterators -----------------//
	template<typename T>
	typename Vector<T>::iterator Vector<T>::begin() noexcept {
		return iterator(m_data);
	}

    template<typename T>
	typename Vector<T>::const_iterator Vector<T>::begin() const noexcept{
		return cbegin();
	}

    template<typename T>
	typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept{
		return const_iterator(m_data);
	}

    template<typename T>
	typename Vector<T>::iterator Vector<T>::end() noexcept {
		return iterator(&m_data[m_size]);
	}

    template<typename T>
	typename Vector<T>::const_iterator Vector<T>::end() const noexcept {
		return cend();
	}

    template<typename T>
	typename Vector<T>::const_iterator Vector<T>::cend() const noexcept{
		return const_iterator(&m_data[m_size]);
	}



    //-----------------  Capacity ------------------//
    template<typename T>
    void Vector<T>::reserve(std::size_t new_cap){
        if (new_cap > m_capacity){
            Vector temp(new_cap);
            for (std::size_t i; i < m_size; ++i){
                temp[i] = m_data[i];
            }
            swap(temp);
        }
    }

    template<typename T>
    std::size_t Vector<T>::size() const{
        return m_size;
    }

    //private function
    template<typename T>
    void Vector<T>::move_data(T *from, T *to, std::size_t count){
        if (from < to){ // for insert operations
            T *_from = from + count - 1, *_to = to + count - 1;
            for (std::size_t i = count; i > 0; --i){
                *_to-- = std::move(*_from--);
            }
            return;
        }

        if (from > to){ // for erase operations
            T *_from = from, *_to = to;
            for (std::size_t i = 0; i < count - 1; ++i){
                *_to++ = std::move(*_from++);
            }
            return;
        }
    }

    //-----------------  Modifiers -----------------//
    template<typename T>
    void Vector<T>::clear() noexcept {
        if (m_data){
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }
    }

    // private function member
    template<typename T>
    T *Vector<T>::check_to_insert(T *pos){
        auto pos_to_check = pos;
        if (pos_to_check < m_data || pos_to_check > &m_data[m_size] ){
           return nullptr;
        }

        if (m_capacity < m_size) {
            reserve (m_capacity*2);
        }

        move_data(pos_to_check, pos_to_check +1, m_size  - (pos_to_check - m_data));
        return pos_to_check;

    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T &value){
        auto to_insert = check_to_insert(pos.m_current);
        if (to_insert != nullptr){
            *to_insert = value;
            ++m_size;
        }
        return iterator{to_insert};
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T &&value) noexcept {
        auto to_insert = check_to_insert(pos.m_current);
        if (to_insert != nullptr){
            *to_insert = std::move(value);
            ++m_size;
        }
        return iterator{to_insert};
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(std::size_t pos, const T &value) {
        return iterator{insert(const_iterator(&m_data[pos]), value)};
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(std::size_t pos, T &&value) {
        return iterator{insert(const_iterator(&m_data[pos]), std::move(value))};
    }


    template<typename T>
    typename Vector<T>::iterator Vector<T>::erase(const_iterator pos){
        auto to_erase = pos.m_current;
        if (to_erase < m_data || to_erase >= &m_data[m_size] ){
            return iterator{};
        }

        move_data(to_erase + 1, to_erase, m_size  - (to_erase - m_data));
        --m_size;
        return iterator{pos.m_current};
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::erase(const std::size_t pos){
        if (pos < m_size){
            return erase(const_iterator(m_data + pos));
        }
        return iterator{};
    }

    template<typename T>
    void Vector<T>::push_back(const T &value){
        if (!m_capacity){
            reserve(5);
        }else if (m_capacity < m_size){
            reserve(2 * m_capacity);
        }

        m_data[m_size++] = value;
    }

    template <typename T>
    void Vector<T>::swap(Vector &vector) noexcept{
    //    auto tmp_data = m_data;
    //    auto tmp_capacity = m_capacity;
    //    auto tmp_size = m_size;

    //    m_data = vector.m_data;
    //    m_size = vector.m_size;
    //    m_capacity = vector.m_size;
       
    //    vector.m_data = tmp_data;
    //    vector.m_size = tmp_capacity;
    //    vector.m_size = tmp_size;
    std::swap(this->m_size, vector.m_size);
    std::swap(this->m_capacity, vector.m_capacity);
    std::swap(this->m_data, vector.m_data);
    }

    //------------------- Operations -----------------------//
    template<typename T>
	std::string Vector<T>::toString(const std::string &name) const {
        std::stringstream stream;
        stream << "\n<===== Vector: " << name << " ======>\n >>Size:" << m_size;
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
	std::ostream& operator<<(std::ostream& os, const Vector<T> & vector) {
		// for (const auto &it : vector) {
		// 	os << it << "->";
		// }
        for (std::size_t i = 0 ; i < vector.size(); ++i) {
			os << vector[i] << ", ";
		}

        os << "END";
            return os;
	}

	//-------------- Inner class const_iterator --------//
    template<class T>
    class Vector<T>::const_iterator {
    public:
        const_iterator();

        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);

        T& operator*();

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;
        std::ptrdiff_t operator-(const const_iterator& other) const;

    protected:
        T* m_current; // member

        const_iterator(T *new_ptr); // constructor
		T &get() const; // get the value at the iterator current position
		friend class Vector<T>;
    };

    //------------------- Inner class iterator ------------------//
	template <typename T>
	class Vector<T>::iterator final: public const_iterator {
	public:
		iterator();

		T &operator*();
		const T &operator*() const;

		iterator &operator++();
		iterator operator++(int);
		iterator &operator--();
		iterator operator--(int);

	private:
		iterator(T *new_ptr); // constructor
		friend class Vector<T>;
	};

    //-------------- class const_iterator implementation--------//
    template<typename T>
    Vector<T>::const_iterator::const_iterator() :m_current{nullptr} {}

    //protected constructor
	template <typename T>
	Vector<T>::const_iterator::const_iterator(T *new_ptr) :m_current{new_ptr} {}

    template<typename T>
    typename Vector<T>::const_iterator &Vector<T>::const_iterator::operator++(){
        ++m_current;
        return *this;
    }

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::const_iterator::operator++(int){
        const_iterator temp = m_current;
        ++m_current;
        return temp;
    }

    template<typename T>
    typename Vector<T>::const_iterator &Vector<T>::const_iterator::operator--(){
        --m_current;
        return *this;
    }

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::const_iterator::operator--(int){
        const_iterator temp = m_current;
        --m_current;
        return temp;
    }

    template<typename T>
    T& Vector<T>::const_iterator::operator*(){
        return get();
    }

    // protected member function
    template<typename T>
    T &Vector<T>::const_iterator::get() const {
        return *m_current;
    }

    template<typename T>
    bool Vector<T>::const_iterator::operator==(const const_iterator &other) const {
        return this->m_current == other.m_current;
    }

    template<typename T>
    bool Vector<T>::const_iterator::operator!=(const const_iterator &other) const {
        return !(*this == other);
    }
    template<typename T>
    std::ptrdiff_t Vector<T>::const_iterator::operator-(const const_iterator &other) const {
        return this->m_current == other.m_current;
    }

    //-------------- class iterator implementation--------//
    template<typename T>
    Vector<T>::iterator::iterator() :const_iterator{}{}

    //protected constructor
	template <typename T>
	Vector<T>::iterator::iterator(T *new_ptr) :const_iterator{new_ptr} {}

     template<typename T>
    typename Vector<T>::iterator &Vector<T>::iterator::operator++(){
        ++(this->m_current);
        return *this;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::iterator::operator++(int){
        iterator temp = this->m_current;
         ++(this->m_current);
        return temp;
    }

    template<typename T>
    typename Vector<T>::iterator &Vector<T>::iterator::operator--(){
        --(this->m_current);
        return *this;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::iterator::operator--(int){
        iterator temp = this->m_current;
        --(this->m_current);
        return temp;
    }

	template <typename T>
	const T &Vector<T>::iterator::operator*() const {
		return const_iterator::operator*();
	}

	template <typename T>
	T &Vector<T>::iterator::operator*() {
		return const_iterator::get();
	}





} // namespace container
