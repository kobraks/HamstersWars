#include "stdafx.h"
#include "IProperty.h"

namespace game::interfaces
{
	IProperty::IProperty(const std::string& type_name, const property_id_type& property_id) : type_(Type_t(type_name)),
	                                                                                          id_(property_id)
	{
	}

	IProperty::~IProperty() = default;

	const IProperty::Type_t* IProperty::type() const
	{
		return &type_;
	}

	const property_id_type& IProperty::id() const
	{
		return id_;
	}

	void IProperty::set_type(const std::string& type_name)
	{
		type_ = Type_t(type_name);
	}
};
