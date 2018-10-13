#pragma once
#include <vector>

namespace game
{
	template<class ...Params>
	class LuaAction
	{
	public:
		typedef LuaIntf::LuaRef action;

	private:
		typedef std::vector<action> table_type;

	public:
		typedef table_type::size_type size_type;

		class LuaActionIterator
		{
		public:

			typedef LuaActionIterator self_type;
			typedef action value_type;
			typedef action& reference;
			typedef action* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit LuaActionIterator(typename table_type::iterator iterator);

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

			friend class LuaAction;
		};

		class ConstLuaActionIterator
		{
		public:
			typedef ConstLuaActionIterator self_type;
			typedef action value_type;
			typedef action& reference;
			typedef action* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ConstLuaActionIterator(typename table_type::const_iterator iterator);

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

			friend class LuaAction;
		};

		LuaAction() = default;
		explicit LuaAction(const action& event);
		template<class T>
		LuaAction(const T& begin, const T& end);

		LuaAction(const LuaAction&) = delete;
		LuaAction(LuaAction&&) = delete;
		LuaAction& operator= (const LuaAction&) = delete;
		LuaAction& operator= (LuaAction&&) = delete;

		action operator[](const size_type& index) const;
		action& operator[](const size_type& index);

		action get(const size_type& index) const;
		action& get(const size_type& index);

		void operator()(Params&... params) const;
		LuaAction& operator+=(const action& action);

		void call(Params&... params) const;

		void add(const action& action);

		action get_first() const;
		size_type size() const;

		LuaActionIterator begin() noexcept;
		LuaActionIterator end() noexcept;

		ConstLuaActionIterator begin() const noexcept;
		ConstLuaActionIterator end() const noexcept;

		void remove(size_type index);
		void remove(LuaActionIterator iterator);
	private:
		table_type actions_;
	};

	template <class ... Params>
	LuaAction<Params...>::LuaActionIterator::LuaActionIterator(table_type::iterator iterator) : current_(iterator)
	{
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator::self_type LuaAction<Params...>::LuaActionIterator::operator++()
	{
		++current_;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator::self_type LuaAction<Params...>::LuaActionIterator::operator
	++(int i)
	{
		current_ += i;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator::self_type LuaAction<Params...>::LuaActionIterator::operator--()
	{
		--current_;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator::self_type LuaAction<Params...>::LuaActionIterator::operator
	--(int i)
	{
		current_ -= i;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator::reference LuaAction<Params...>::LuaActionIterator::operator
	*() const
	{
		return *current_;
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator::pointer LuaAction<Params...>::LuaActionIterator::operator
	->() const
	{
		return &*current_;
	}

	template <class ... Params>
	bool LuaAction<Params...>::LuaActionIterator::operator==(const self_type& right) const
	{
		return current_ == right.current_;
	}

	template <class ... Params>
	bool LuaAction<Params...>::LuaActionIterator::operator!=(const self_type& right) const
	{
		return current_ != right.current_;
	}

	template <class ... Params>
	LuaAction<Params...>::ConstLuaActionIterator::ConstLuaActionIterator(table_type::const_iterator iterator) : current_(iterator)
	{
	}

	template <class ... Params>
	typename LuaAction<Params...>::ConstLuaActionIterator::self_type LuaAction<Params...>::ConstLuaActionIterator::
	operator++()
	{
		++current_;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::ConstLuaActionIterator::self_type LuaAction<Params...>::ConstLuaActionIterator::
	operator++(int i)
	{
		current_ += i;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::ConstLuaActionIterator::self_type LuaAction<Params...>::ConstLuaActionIterator::
	operator--()
	{
		--current_;
		return *this;
	}

	template <class ... Params>
	typename LuaAction<Params...>::ConstLuaActionIterator::self_type LuaAction<Params...>::ConstLuaActionIterator::
	operator--(int i)
	{
		current_ -= i;
		return *this;
	}

	template <class ... Params>
	const typename LuaAction<Params...>::action& LuaAction<Params...>::ConstLuaActionIterator::operator*() const
	{
		return *current_;
	}

	template <class ... Params>
	const typename LuaAction<Params...>::action* LuaAction<Params...>::ConstLuaActionIterator::operator->() const
	{
		return &*current_;
	}

	template <class ... Params>
	bool LuaAction<Params...>::ConstLuaActionIterator::operator==(const self_type& right) const
	{
		return current_ == right.current_;
	}

	template <class ... Params>
	bool LuaAction<Params...>::ConstLuaActionIterator::operator!=(const self_type& right) const
	{
		return current_ != right.current_;
	}

	template <class ... Params>
	LuaAction<Params...>::LuaAction(const action& event)
	{
		add(event);
	}

	template <class ... Params>
	template <class T>
	LuaAction<Params...>::LuaAction(const T& begin, const T& end)
	{
		for (T current = begin; current != end; ++current)
			add(current);
	}

	template <class ... Params>
	typename LuaAction<Params...>::action LuaAction<Params...>::operator[](const size_type& index) const
	{
		return get(index);
	}

	template <class ... Params>
	typename LuaAction<Params...>::action& LuaAction<Params...>::operator[](const size_type& index)
	{
		return get(index);
	}

	template <class ... Params>
	typename LuaAction<Params...>::action LuaAction<Params...>::get(const size_type& index) const
	{
		assert(index > 0 && index < actions_.size());
		return actions_[index];
	}

	template <class ... Params>
	typename LuaAction<Params...>::action& LuaAction<Params...>::get(const size_type& index)
	{
		assert(index > 0 && index < actions_.size());
		return actions_[index];
	}

	template <class ... Params>
	void LuaAction<Params...>::operator()(Params&... params) const
	{
		call(params...);
	}

	template <class ... Params>
	LuaAction<Params...>& LuaAction<Params...>::operator+=(const action& action)
	{
		add(action);
		return *this;
	}

	template <class ... Params>
	void LuaAction<Params...>::call(Params&... params) const
	{
		for (auto function : actions_)
			function.call(params...);
	}

	template <class ... Params>
	void LuaAction<Params...>::add(const action& action)
	{
		actions_.push_back(action);
	}

	template <class ... Params>
	typename LuaAction<Params...>::action LuaAction<Params...>::get_first() const
	{
		return actions_[0];
	}

	template <class ... Params>
	typename LuaAction<Params...>::size_type LuaAction<Params...>::size() const
	{
		return actions_.size();
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator LuaAction<Params...>::begin() noexcept
	{
		return LuaActionIterator(actions_.begin());
	}

	template <class ... Params>
	typename LuaAction<Params...>::LuaActionIterator LuaAction<Params...>::end() noexcept
	{
		return LuaActionIterator(actions_.end());
	}

	template <class ... Params>
	typename LuaAction<Params...>::ConstLuaActionIterator LuaAction<Params...>::begin() const noexcept
	{
		return ConstLuaActionIterator(actions_.begin());
	}

	template <class ... Params>
	typename LuaAction<Params...>::ConstLuaActionIterator LuaAction<Params...>::end() const noexcept
	{
		return ConstLuaActionIterator(actions_.end());
	}

	template <class ... Params>
	void LuaAction<Params...>::remove(size_type index)
	{
		assert(index >= 0 && index < actions_.size());
		actions_.erase(actions_.begin() + index);
	}

	template <class ... Params>
	void LuaAction<Params...>::remove(LuaActionIterator iterator)
	{
		assert(iterator != end());
		actions_.erase(iterator.current_);
	}
}
