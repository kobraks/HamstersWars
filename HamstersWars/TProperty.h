#pragma once
#include "IProperty.h"

namespace game::interfaces
{
	template<class Type>
	class TProperty : public IProperty
	{
	public:
		typedef Type value_type;

		explicit TProperty(const property_id_type& property_id);
		TProperty(const property_id_type& property_id, value_type value);

		virtual ~TProperty() = default;

		value_type get_value() const;

		void set_value(value_type value);

		void update() override;
		IProperty* clone() const override;

		const value_type& value;
	private:
		value_type value_;
	};

	template <class Type>
	TProperty<Type>::TProperty(const property_id_type& property_id) : IProperty(GET_TYPE_NAME(value_type), property_id),
	                                                                  value(value_)
	{
	}

	template <class Type>
	TProperty<Type>::
	TProperty(const property_id_type& property_id, value_type value) :
		IProperty(GET_TYPE_NAME(value_type), property_id), value(value_), value_(std::move(value))
	{
	}

	template <class Type>
	typename TProperty<Type>::value_type TProperty<Type>::get_value() const
	{
		return value_;
	}

	template <class Type>
	void TProperty<Type>::set_value(value_type value)
	{
		value_ = value;
	}

	template <class Type>
	void TProperty<Type>::update()
	{
	}

	template <class Type>
	IProperty* TProperty<Type>::clone() const
	{
		return new TProperty<Type>(id(), value_);
	}
}

