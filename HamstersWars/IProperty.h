#pragma once
#include "Defines.h"
#include "Types.h"

namespace game::interfaces
{
	class IProperty
	{
	public:
		class Type_t
		{
			std::string name_;
		public:
			Type_t(const std::string& name) : name_(name)
			{}

			std::string name() const
			{
				return name_;
			}
		};

		IProperty(const std::string& type_name, const property_id_type& property_id);
		virtual ~IProperty();

		const Type_t* type() const;
		const property_id_type& id() const;

		virtual void update() = 0;
		virtual IProperty* clone() const = 0;

	protected:
		void set_type(const std::string& type_name);

	private:
		Type_t type_;
		const property_id_type id_;
	};
}

