#pragma once
#include <iterator>
#include <cassert>

template<class T>
class Stack
{
public:
	typedef T value_type;
	typedef size_t size_type;

private:
	class StackElement
	{
	public:
		StackElement();
		explicit StackElement(const value_type& value);

		StackElement* get_next() const;
		StackElement*& get_next();

		value_type& get_vale();
		value_type get_vale() const;

		void set_value(const value_type& value);
		void set_next(StackElement* next);

		value_type operator*() const;
		value_type& operator*();
	private:
		value_type value_;
		StackElement* next_;

		friend class Stack;
	};
public:

	class StackIterator
	{
		typedef StackIterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t difference_type;

		explicit StackIterator(StackElement* element);

		self_type& operator++();
		self_type& operator++(int i);

		reference operator*() const;
		pointer operator->() const;

		bool operator==(const self_type& right) const;
		bool operator!=(const self_type& right) const;
	private:
		StackElement* element_;

		friend class Stack;
	};

	class ConstStackIterator
	{
		typedef ConstStackIterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t difference_type;

		explicit ConstStackIterator(StackElement* element);

		self_type& operator++();
		self_type& operator++(int i);

		const value_type& operator*() const;
		const value_type* operator->() const;

		bool operator==(const self_type& right) const;
		bool operator!=(const self_type& right) const;
	private:
		StackElement* element_;

		friend class Stack;
	};

	Stack();

	template<class B>
	Stack(const B& begin, const B& end);

	void push(const value_type& value);

	template<class B>
	void push(const B& begin, const B& end);

	value_type& pop();

	value_type& top();
	value_type top() const;

	bool empty() const;
	size_type size() const;

	void clear();

	StackIterator begin() noexcept;
	StackIterator end() noexcept;

	ConstStackIterator begin() const noexcept;
	ConstStackIterator end() const noexcept;

	~Stack();
private:
	StackElement* first_ = nullptr;
	StackElement* last_ = nullptr;
	size_type size_ = 0;
};

template <class T>
Stack<T>::StackElement::StackElement(): next_(nullptr)
{
}

template <class T>
Stack<T>::StackElement::StackElement(const value_type& value) : value_(value)
{
}

template <class T>
typename Stack<T>::StackElement* Stack<T>::StackElement::get_next() const
{
	return next_;
}

template <class T>
typename Stack<T>::StackElement*& Stack<T>::StackElement::get_next()
{
	return next_;
}

template <class T>
typename Stack<T>::value_type& Stack<T>::StackElement::get_vale()
{
	return value_;
}

template <class T>
typename Stack<T>::value_type Stack<T>::StackElement::get_vale() const
{
	return value_;
}

template <class T>
void Stack<T>::StackElement::set_value(const value_type& value)
{
	value_ = value;
}

template <class T>
void Stack<T>::StackElement::set_next(StackElement* next)
{
	next_ = next;
}

template <class T>
typename Stack<T>::value_type Stack<T>::StackElement::operator*() const
{
	return value_;
}

template <class T>
typename Stack<T>::value_type& Stack<T>::StackElement::operator*()
{
	return value_;
}

template <class T>
Stack<T>::StackIterator::StackIterator(StackElement* element) : element_(element)
{
}

template <class T>
typename Stack<T>::StackIterator::self_type& Stack<T>::StackIterator::operator++()
{
	if (element_ != nullptr)
		element_ = element_->get_next();
	return *this;
}

template <class T>
typename Stack<T>::StackIterator::self_type& Stack<T>::StackIterator::operator++(int i)
{
	for (auto int j = 0; j < i; ++j)
	{
		if (element_ == nullptr)
			break;
		element_ = element_->get_next();
	}

	return *this;
}

template <class T>
typename Stack<T>::StackIterator::reference Stack<T>::StackIterator::operator*() const
{
	return element_->get_value();
}

template <class T>
typename Stack<T>::StackIterator::pointer Stack<T>::StackIterator::operator->() const
{
	return *element_->get_value();
}

template <class T>
bool Stack<T>::StackIterator::operator==(const self_type& right) const
{
	return element_ == right.element_;
}

template <class T>
bool Stack<T>::StackIterator::operator!=(const self_type& right) const
{
	return element_ != right.element_;
}

template <class T>
Stack<T>::ConstStackIterator::ConstStackIterator(StackElement* element) : element_(element)
{
}

template <class T>
typename Stack<T>::ConstStackIterator::self_type& Stack<T>::ConstStackIterator::operator++()
{
	if (element_ != nullptr)
		element_ = element_->get_next();
	return *this;
}

template <class T>
typename Stack<T>::ConstStackIterator::self_type& Stack<T>::ConstStackIterator::operator++(int i)
{
	for (auto int j = 0; j < i; ++j)
	{
		if (element_ == nullptr)
			break;
		element_ = element_->get_next();
	}
	return *this;
}

template <class T>
const typename Stack<T>::ConstStackIterator::value_type& Stack<T>::ConstStackIterator::operator*() const
{
	return element_->get_value();
}

template <class T>
const typename Stack<T>::ConstStackIterator::value_type* Stack<T>::ConstStackIterator::operator->() const
{
	return *element_->get_value();
}

template <class T>
bool Stack<T>::ConstStackIterator::operator==(const self_type& right) const
{
	return element_ == right.element_;
}

template <class T>
bool Stack<T>::ConstStackIterator::operator!=(const self_type& right) const
{
	return element_ != right.element_;
}

template <class T>
Stack<T>::Stack()
{
}

template <class T>
template <class B>
Stack<T>::Stack(const B& begin, const B& end)
{
	push(begin, end);
}

template <class T>
void Stack<T>::push(const value_type& value)
{
	size_++;

	if (!first_)
	{
		last_ = first_ = new  StackElement(value);
		first_->set_next(last_);
		return;
	}

	auto last = last_;
	last_ = new StackElement(value);
	last->set_next(last_);
}

template <class T>
template <class B>
void Stack<T>::push(const B& begin, const B& end)
{
	for (B current = begin; current != end; ++current)
		push(*current);
}

template <class T>
typename Stack<T>::value_type& Stack<T>::pop()
{
	assert(!empty());
	
	size_--;
	value_type value = first_->get_vale();
	auto remove = first_;


	if (first_ == last_)
	{
		delete first_;
		first_ = last_ = nullptr;
	}
	else
	{
		first_ = first_->get_next();
		delete remove;
	}

	return value;
}

template <class T>
typename Stack<T>::value_type& Stack<T>::top()
{
	assert(!empty());
	return *first_;
}

template <class T>
typename Stack<T>::value_type Stack<T>::top() const
{
	assert(!empty());
	return *first_;
}

template <class T>
bool Stack<T>::empty() const
{
	return !first_;
}

template <class T>
typename Stack<T>::size_type Stack<T>::size() const
{
	return size_;
}

template <class T>
void Stack<T>::clear()
{
	if (empty())
		return;

	if (first_ == last_)
	{
		delete first_;
		first_ = last_ = nullptr;

		return;
	}

	auto next = first_->get_next();

	for (auto i = first_; i != last_; i = next)
	{
		next = i->get_next();
		delete i;
	}

	first_ = last_ = nullptr;
	size_ = 0;
}

template <class T>
typename Stack<T>::StackIterator Stack<T>::begin() noexcept
{
	return StackIterator(first_);
}

template <class T>
typename Stack<T>::StackIterator Stack<T>::end() noexcept
{
	return StackIterator(last_);
}

template <class T>
typename Stack<T>::ConstStackIterator Stack<T>::begin() const noexcept
{
	return ConstStackIterator(first_);
}

template <class T>
typename Stack<T>::ConstStackIterator Stack<T>::end() const noexcept
{
	return ConstStackIterator(last_);
}

template <class T>
Stack<T>::~Stack()
{
	clear();
}
