/*
#include "Action.h"
#include <cassert>


game::Action::ActionIterator::ActionIterator(table_type::iterator iterator) : current_(iterator)
{

}

game::Action::ActionIterator::self_type game::Action::ActionIterator::operator++()
{
	++current_;
	return *this;
}

game::Action::ActionIterator::self_type game::Action::ActionIterator::operator++(int i)
{
	current_ += i;

	return *this;
}

game::Action::ActionIterator::self_type game::Action::ActionIterator::operator--()
{
	--current_;

	return *this;
}

game::Action::ActionIterator::self_type game::Action::ActionIterator::operator--(int i)
{
	current_ -= i;

	return *this;
}

game::Action::ActionIterator::reference game::Action::ActionIterator::operator*() const
{
	return *current_;
}

game::Action::ActionIterator::pointer game::Action::ActionIterator::operator->() const
{
	return &(*current_);
}

bool game::Action::ActionIterator::operator==(const self_type& right) const
{
	return current_ == right.current_;
}

bool game::Action::ActionIterator::operator!=(const self_type& right) const
{
	return current_ != right.current_;
}

game::Action::ConstActionIterator::ConstActionIterator(table_type::const_iterator iterator) : current_(iterator)
{
}

game::Action::ConstActionIterator::self_type game::Action::ConstActionIterator::operator++()
{
	++current_;

	return *this;
}

game::Action::ConstActionIterator::self_type game::Action::ConstActionIterator::operator++(int i)
{
	current_ += i;

	return *this;
}

game::Action::ConstActionIterator::self_type game::Action::ConstActionIterator::operator--()
{
	--current_;

	return *this;
}

game::Action::ConstActionIterator::self_type game::Action::ConstActionIterator::operator--(int i)
{
	current_ -= i;

	return *this;
}

const game::Action::action& game::Action::ConstActionIterator::operator*() const
{
	return *current_;
}

const game::Action::action* game::Action::ConstActionIterator::operator->() const
{
	return &(*current_);
}

bool game::Action::ConstActionIterator::operator==(const self_type& right) const
{
	return current_ == right.current_;
}

bool game::Action::ConstActionIterator::operator!=(const self_type& right) const
{
	return current_ != right.current_;
}

game::Action::Action()
{
}


game::Action::~Action()
{
	
}

game::Action::action game::Action::operator[](const size_type& index) const
{
	return get(index);
}

game::Action::action& game::Action::operator[](const size_type& index)
{
	return get(index);
}

game::Action::action game::Action::get(const size_type& index) const
{
	assert(index >= 0 && index < actions_.size());

	return actions_[index];
}

game::Action::action& game::Action::get(const size_type& index)
{
	assert(index >= 0 && index < actions_.size());

	return actions_[index];
}

void game::Action::operator()(void** params) const
{
	for (auto func : actions_)
		func(params);
}

game::Action& game::Action::operator+=(const action& action)
{
	add(action);
}

void game::Action::add(const action& action)
{
	actions_.push_back(action);
}

game::Action::action game::Action::get_first() const
{
	return actions_[0];
}

game::Action::size_type game::Action::size() const
{
	return actions_.size();
}

game::Action::ActionIterator game::Action::begin() noexcept
{
	return ActionIterator(actions_.begin());
}

game::Action::ActionIterator game::Action::end() noexcept
{
	return ActionIterator(actions_.end());
}

game::Action::ConstActionIterator game::Action::begin() const noexcept
{
	return ConstActionIterator(actions_.begin());
}

game::Action::ConstActionIterator game::Action::end() const noexcept
{
	return ConstActionIterator(actions_.end());
}

void game::Action::remove(size_type index)
{
	assert(index >= 0 && index < actions_.size());


	actions_.erase(actions_.begin() + index);
}

void game::Action::remove(ActionIterator iterator)
{
	assert(iterator.current_ != actions_.end());

	actions_.erase(iterator.current_);
}

game::Action::Action(const action& event)
{
	actions_.push_back(event	);
}
*/
