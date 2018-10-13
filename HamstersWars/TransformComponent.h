#pragma once
#include "Component.h"

namespace game
{
	namespace component
	{
		class TransformComponent : public interfaces::Component
		{
		public:
			TransformComponent();
			~TransformComponent();

			void parse_table(LuaIntf::LuaRef table) override;
			Component* clone() const override;

			void set_position(const gl::Vector3D& position);
			gl::Vector3D get_position() const;

			void set_scale(const gl::Vector3D& position);
			gl::Vector3D get_scale() const;

			void set_rotation(const gl::Vector3D& rotation);
			gl::Vector3D get_rotation() const;
		};
	}
}

