#pragma once
#include <vector>
#include <functional>

namespace game
{
	template<class ...Params>
	class Action 
	{
	public:
		typedef std::function<void(Params...)> action;
		typedef size_t size_type;

	private:
		typedef std::vector<action> table_type;

	public:
		class ActionIterator
		{
		public:

			typedef ActionIterator self_type;
			typedef action value_type;
			typedef action& reference;
			typedef action* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ActionIterator(typename table_type::iterator iterator);

			self_type operator++();
			self_type operator++(int i);

			self_type operator--();
			self_type operator--(int i);

			reference operator*() const;
			pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			typename table_type::iterator current_;

			friend class Action;
		};

		class ConstActionIterator
		{
		public:
			typedef ConstActionIterator self_type;
			typedef action value_type;
			typedef action& reference;
			typedef action* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ConstActionIterator(typename table_type::const_iterator iterator);

			self_type operator++();
			self_type operator++(int i);

			self_type operator--();
			self_type operator--(int i);

			const action& operator*() const;
			const action* operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			typename table_type::const_iterator current_;

			friend class Action;
		};

		Action() = default;
		explicit Action(const action& event);
		template<class T>
		Action(const T& begin, const T& end);

		Action(const Action&) = delete;
		Action(Action&&) = delete;
		Action& operator= (const Action&) = delete;
		Action& operator= (Action&&) = delete;

		action operator[](const size_type& index) const;
		action& operator[](const size_type& index);

		action get(const size_type& index) const;
		action& get(const size_type& index);

		void operator()(Params&&... params) const;
		Action& operator+=(const action& action);

		void add(const action& action);

		action get_first() const;
		size_type size() const;

		ActionIterator begin() noexcept;
		ActionIterator end() noexcept;

		ConstActionIterator begin() const noexcept;
		ConstActionIterator end() const noexcept;

		void remove(size_type index);
		void remove(ActionIterator iterator);
	private:
		table_type actions_;
	};

	template <class ... Params>
	Action<Params...>::ActionIterator::ActionIterator(typename table_type::iterator iterator) : current_(iterator)
	{
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator::self_type Action<Params...>::ActionIterator::operator++()
	{
		++current_;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator::self_type Action<Params...>::ActionIterator::operator++(int i)
	{
		current_ += i;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator::self_type Action<Params...>::ActionIterator::operator--()
	{
		--current_;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator::self_type Action<Params...>::ActionIterator::operator--(int i)
	{
		current_ -= i;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator::reference Action<Params...>::ActionIterator::operator*() const
	{
		return *current_;
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator::pointer Action<Params...>::ActionIterator::operator->() const
	{
		return &(*current_);
	}

	template <class ... Params>
	bool Action<Params...>::ActionIterator::operator==(const self_type& right) const
	{
		return current_ == right.current_;
	}

	template <class ... Params>
	bool Action<Params...>::ActionIterator::operator!=(const self_type& right) const
	{
		return current_ != right.current_;
	}

	template <class ... Params>
	Action<Params...>::ConstActionIterator::ConstActionIterator(typename table_type::const_iterator iterator) : current_(iterator)
	{
	}

	template <class ... Params>
	typename Action<Params...>::ConstActionIterator::self_type Action<Params...>::ConstActionIterator::operator++()
	{
		++current_;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ConstActionIterator::self_type Action<Params...>::ConstActionIterator::operator++(int i)
	{
		current_ += i;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ConstActionIterator::self_type Action<Params...>::ConstActionIterator::operator--()
	{
		--current_;
		return *this;
	}

	template <class ... Params>
	typename Action<Params...>::ConstActionIterator::self_type Action<Params...>::ConstActionIterator::operator--(int i)
	{
		current_ -= i;
		return *this;
	}

	template <class ... Params>
	const typename Action<Params...>::action& Action<Params...>::ConstActionIterator::operator*() const
	{
		return *current_;
	}

	template <class ... Params>
	const typename Action<Params...>::action* Action<Params...>::ConstActionIterator::operator->() const
	{
		return &(*current_);
	}

	template <class ... Params>
	bool Action<Params...>::ConstActionIterator::operator==(const self_type& right) const
	{
		return current_ == right.current_;
	}

	template <class ... Params>
	bool Action<Params...>::ConstActionIterator::operator!=(const self_type& right) const
	{
		return current_ != right.current_;
	}

	template <class ... Params>
	Action<Params...>::Action(const action& event)
	{
		add(event);
	}

	template <class ... Params>
	template <class T>
	Action<Params...>::Action(const T& begin, const T& end)
	{
		for (auto current = begin; current != end; ++current)
			actions_.push_back(*current);
	}

	template <class ... Params>
	typename Action<Params...>::action Action<Params...>::operator[](const size_type& index) const
	{
		return get(index);
	}

	template <class ... Params>
	typename Action<Params...>::action& Action<Params...>::operator[](const size_type& index)
	{
		return get(index);
	}

	template <class ... Params>
	typename Action<Params...>::action Action<Params...>::get(const size_type& index) const
	{
		assert(index >= 0 && index < actions_.size());
		return actions_[index];
	}

	template <class ... Params>
	typename Action<Params...>::action& Action<Params...>::get(const size_type& index)
	{
		assert(index >= 0 && index < actions_.size());
		return actions_[index];
	}

	template <class ... Params>
	void Action<Params...>::operator()(Params&&... params) const
	{
		for (auto func : actions_)
			func(params...);
	}

	template <class ... Params>
	Action<Params...>& Action<Params...>::operator+=(const action& action)
	{
		add(action);
		return *this;
	}

	template <class ... Params>
	void Action<Params...>::add(const action& action)
	{
		actions_.push_back(action);
	}

	template <class ... Params>
	typename Action<Params...>::action Action<Params...>::get_first() const
	{
		return actions_[0];
	}

	template <class ... Params>
	typename Action<Params...>::size_type Action<Params...>::size() const
	{
		return actions_.size();
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator Action<Params...>::begin() noexcept
	{
		return ActionIterator(actions_.begin());
	}

	template <class ... Params>
	typename Action<Params...>::ActionIterator Action<Params...>::end() noexcept
	{
		return ActionIterator(actions_.end());
	}

	template <class ... Params>
	typename Action<Params...>::ConstActionIterator Action<Params...>::begin() const noexcept
	{
		return ConstActionIterator(actions_.begin());
	}

	template <class ... Params>
	typename Action<Params...>::ConstActionIterator Action<Params...>::end() const noexcept
	{
		return ConstActionIterator(actions_.end());
	}

	template <class ... Params>
	void Action<Params...>::remove(size_type index)
	{
		assert(index >= 0 && index < actions_.size());

		actions_.erase(actions_.begin() + index);
	}

	template <class ... Params>
	void Action<Params...>::remove(ActionIterator iterator)
	{
		assert(iterator != end());

		actions_.erase(iterator.current_);
	}
}